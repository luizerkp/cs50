import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, fail_password, round_seconds
from datetime import datetime
import re

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""
    current_stocks = db.execute("SELECT stock_symbol, shares_held FROM portfolio WHERE user_id = ?", session["user_id"])
    stocks_list = []
    stock_sum = 0
    flash(current_stocks)
    # loop through current_stocks list getting current price
    for stock in current_stocks:
        for key, value in stock.items():
            if key == "stock_symbol":
                stock_quote = lookup(stock["stock_symbol"])
                total_stock_value = stock_quote["price"] * float(stock["shares_held"])
                stock_sum = stock_sum + total_stock_value
                stock_dict = {"symbol": stock_quote["symbol"],
                              "name": stock_quote["name"],
                              "shares": stock["shares_held"],
                              "price": stock_quote["price"],
                              "TOTAL": total_stock_value}
                stocks_list.append(stock_dict)

    # findout how much cash is remaining
    rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    # users current availavle cash
    cash = rows[0]["cash"]

    # Users total wealth
    portfolio_sum = cash + stock_sum

    return render_template("index.html", stocks_list=stocks_list, cash=cash, portfolio_sum=portfolio_sum)


@app.route("/buy", methods=["GET", "POST"])
@app.route("/buy/<index_stock_symbol>", methods=["GET", "POST"])
@login_required
def buy(index_stock_symbol=None):
    """Buy shares of stock"""
    # If request route is POST check attempt to retrieve quote
    if request.method == "POST":
        # get the submitted user input
        stock_symbol = request.form.get("symbol")
        shares_number = request.form.get("shares")
        is_it_int = re.match("[+]?\d+$", shares_number)

        # findout how much cash is remaining
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]

        # if user input is empty return apology
        if not stock_symbol:
            return apology("Missing stock symbol")
        if not shares_number:
            return apology("Missing number of shares to buy")
        if not is_it_int or int(shares_number) == 0:
            return apology("Only positive intergers allowed i.e 1 not 1.0, 0.5, 1.5, -1.... ect")

        # look up current stock value from https://iexcloud.io/ through lookup function
        stock_quote = lookup(stock_symbol)

        # if lookup returns null return apology
        if not stock_quote:
            return apology("Invalid stock symbol")

        # current total value of the stocks to be purchased
        total_purchase_price = stock_quote["price"] * int(shares_number)

        # return apology if user does not have enough cash
        if total_purchase_price > cash:
            return apology("Not enough cash to perform transaction")

        # Update users portfolio
        else:
            now = datetime.now()
            now = round_seconds(now)
            # checks if user already owns the stock to be purchased
            is_in_portfolio = db.execute(
                "SELECT stock_symbol FROM portfolio WHERE stock_symbol = ? AND user_id = ?", stock_quote["symbol"], session["user_id"])

            # if user doesn't already own the stock add stock to portfolio
            if not is_in_portfolio:
                portfolio = db.execute("INSERT INTO portfolio(user_id, stock_symbol, company_name, shares_held) VALUES(?, ?, ?, ?)",
                                       session["user_id"], stock_quote["symbol"], stock_quote["name"], shares_number)
            # otherwise update shares_held number
            else:
                current_shares = db.execute(
                    "SELECT shares_held FROM portfolio WHERE stock_symbol = ? AND user_id = ?", stock_quote["symbol"], session["user_id"])
                total_shares = current_shares[0]["shares_held"] + int(shares_number)
                update_portfolio = db.execute(
                    "UPDATE portfolio SET shares_held = ? WHERE user_id = ? AND stock_symbol = ?", total_shares, session["user_id"], stock_quote["symbol"])

            # deduct total purchase amount from user's remaing cash and update current cash in db
            remaining_cash = cash - total_purchase_price
            update_user_cash = db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_cash, session["user_id"])

            # log the transaction deatails into the transactions table
            log_transaction = db.execute("INSERT INTO transactions(user_id, stock_symbol, shares, price, transaction_date) VALUES(?, ?, ?, ?, ?)",
                                         session["user_id"], stock_quote["symbol"], shares_number, stock_quote["price"], str(now))
            flash(f"Purchase Sucessful: { int(shares_number) } shares of { stock_quote['symbol'] }")
            return redirect("/")
    else:
        return render_template("buy.html", index_stock_symbol=index_stock_symbol)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    stock_history = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    return render_template("history.html", stock_history=stock_history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # If request route is POST check attempt to retrieve quote
    if request.method == "POST":
        # get the user input submitted
        stock_input = request.form.get("symbol")
        # if user input is empty return apology
        if not stock_input:
            return apology("Missing stock symbol")
        stock_quote = lookup(stock_input)
        # if lookup returns null return apology
        if not stock_quote:
            return apology("Invalid stock symbol")
        # display lookup results
        else:
            return render_template("quoted.html", stock_quote=stock_quote)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # If request route is POST attempt to register
    if request.method == "POST":
        # the values for username, password and comfirm password
        username = request.form.get("username")
        password = request.form.get("password")
        confirm_password = request.form.get("confirmation")

        # query the db to ensure username doesn't already exist
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        username_already_exists = None

        # if rows returns a value asign username to varibale
        if rows:
            username_already_exists = rows[0]["username"]

        # sanity checks to for missing fields and not matching passwords and return apology
        if not username:
            return apology("Invalid username", 400)
        if username == username_already_exists:
            return apology("Username already exists please try a different one")
        if not password:
            return apology("Invalid password", 400)
        if password != confirm_password:
            return apology("Passwords do not match", 400)

        # checks that password meets minimum requirements
        bad_password = fail_password(password)

        if bad_password:
            return apology("Password must be at least 8 characters long and contain: 1 special character: @_!#$%^& , 1 number and 1 uppercase letter")

        # create add new user to db and login user
        hashed_password = generate_password_hash(password)
        new_user = db.execute("INSERT INTO users(username, hash) VALUES(?, ?)", username, hashed_password)
        session["user_id"] = new_user
        flash("You are regisred")
        return redirect("/")
    # if rout is GET display the register form.
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@app.route("/sell/<index_stock_symbol>", methods=["GET", "POST"])
@login_required
def sell(index_stock_symbol=None):
    """Sell shares of stock"""
    # If request route is POST check attempt to retrieve quote
    if request.method == "POST":
        # get the submitted user input
        stock_symbol = request.form.get("symbol")
        shares_number = request.form.get("shares")
        is_it_int = re.match("[+]?\d+$", shares_number)

        # validate user input
        if not stock_symbol:
            return apology("Please Select stock symbol")
        if not shares_number:
            return apology("Missing number of shares to sell")
        if not is_it_int or int(shares_number) == 0:
            return apology("Only positive intergers allowed i.e 1 not 1.0, 0.5, 1.5, -1.... ect")

        # look up symbol and get stock info
        stock_quote = lookup(stock_symbol)

        # if lookup return null render apology
        if not stock_quote:
            return apology("Invalid Stock symbol")

        # query db for number of shares owned
        shares_in_portfolio = db.execute(
            "SELECT shares_held FROM portfolio WHERE stock_symbol = ? AND user_id = ?", stock_quote["symbol"], session["user_id"])

        # query returns null render apology
        if not shares_in_portfolio:
            return apology(f"Sorry you don't seem to own any shares in {stock_quote['name']}")

        # render apology if number of shares being sold is higher then shares owned
        if int(shares_number) > shares_in_portfolio[0]["shares_held"]:
            return apology("The number shares being sold is higher than shares owned")

        # calculate remaining shares
        remaining_shares = shares_in_portfolio[0]["shares_held"] - int(shares_number)

        # update users cash
        cash_sale = int(shares_number) * stock_quote["price"]
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        updated_cash = rows[0]["cash"] + cash_sale
        update_user_cash = db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, session["user_id"])

        # get time and set shares sold to negative for db
        now = datetime.now()
        now = round_seconds(now)
        sold = int(shares_number)
        sold = -abs(sold)

        if remaining_shares == 0:
            update_portfolio = db.execute("DELETE FROM portfolio WHERE stock_symbol = ? AND user_id = ?",
                                          stock_quote["symbol"], session["user_id"])
        else:
            update_portfolio = db.execute("UPDATE portfolio SET shares_held = ? WHERE user_id = ? AND stock_symbol = ?",
                                          remaining_shares, session["user_id"], stock_quote["symbol"])

        # log the transaction deatails into the transactions table
        log_transaction = db.execute("INSERT INTO transactions(user_id, stock_symbol, shares, price, transaction_date) VALUES(?, ?, ?, ?, ?)",
                                     session["user_id"], stock_quote["symbol"], sold, stock_quote["price"], str(now))

        flash(f"Sale Successful: { sold } shares of { stock_quote['symbol'] }")
        return redirect("/")
    # render template passing index_stock_symbol as argument
    else:
        current_stocks = db.execute("SELECT stock_symbol FROM portfolio WHERE user_id = ?", session["user_id"])
        symbols = [find["stock_symbol"] for find in current_stocks]
        return render_template("sell.html", index_stock_symbol=index_stock_symbol, symbols=symbols)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    # If request route is POST attempt to change password
    if request.method == "POST":
        # the values for password and comfirm password
        input_current_password = request.form.get("current_password")
        password = request.form.get("password")
        confirm_password = request.form.get("confirm_password")

        # checks that password meets minimum requirements
        bad_password = fail_password(password)

        # sanity checks to for missing fields and not matching passwords and return apology
        if not input_current_password:
            return apology("Missing current password", 403)
        if not password:
            return apology("Missing Password", 403)
        if password != confirm_password:
            return apology("Passwords do not match", 403)
        if bad_password:
            return apology("Password must be at least 8 characters long and contain: 1 special character: @_!#$%^& , 1 number and 1 uppercase letter")
        # hashes inputed new password
        hashed_new_password = generate_password_hash(password)

        # query the db to ensure that new password and old password are not the same
        rows = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])

        current_password = rows[0]["hash"]

        # checks if the current password provided is valid
        if not check_password_hash(current_password, input_current_password):
            return apology("Current password does not match our records")
        # check if the new password is the same as the current password
        if check_password_hash(current_password, password):
            return apology("Please enter a new password")

        # update password
        new_password = db.execute("UPDATE users SET hash = ? WHERE id = ?", hashed_new_password, session["user_id"])
        flash("Password change sucessful")
        return redirect("/")
    else:
        return render_template("change_password.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
