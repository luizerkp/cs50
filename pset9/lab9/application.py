import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for, flash

# Configure application
app = Flask(__name__)
app.secret_key = "super secret key"

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # get name, month, and day from html form input data
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        # gives error in one of the fields (name, month or day) is left empty and redirects to index.html
        if not name or not month or not day:
            flash("Error: Unable to add birthday due to missing Field! Make sure all boxes are filled out and try again")
            return redirect(url_for("index"))

        # Add the user's entry into the database
        else:
            flash("Entry Sucessful!!")
            birthdays = db.execute("INSERT INTO birthdays(name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")

    # displays all birthdays in db if request method is GET
    else:
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays)

# Deletes birthday when delete button is clicked on page and redirects to index.html
@app.route("/delete/<int:birthday_id>/", methods=["POST"])
def delete(birthday_id):
    birthdays = db.execute("DELETE FROM birthdays WHERE birthdays.id = ?", birthday_id)
    flash("Delete Sucessful!!")
    return redirect(url_for("index"))
