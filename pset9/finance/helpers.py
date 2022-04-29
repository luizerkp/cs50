import os
import requests
import urllib.parse
import re
import datetime as dt

from flask import redirect, render_template, request, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        url = f"https://cloud.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


def fail_password(password):
    bad_password = False

    # regex to check for the spcial chars listed
    special_chars = re.compile("[@_!#$%^&]")

    # regex to check for numbers in the str
    numbers = re.compile("[0-9]+")

    # regex to check for uppercase letters
    find_upper = re.compile("[A-Z]")

    if len(password) < 8:
        bad_password = True
    elif not special_chars.search(password):
        bad_password = True
    elif not numbers.search(password):
        bad_password = True
    elif not find_upper.search(password):
        bad_password = True

    return bad_password


def round_seconds(obj: dt.datetime) -> dt.datetime:
    # rounds datetime object to nearest second
    # electrovir's answer from https://stackoverflow.com/questions/47792242/rounding-time-off-to-the-nearest-second-python
    if obj.microsecond >= 500_000:
        obj += dt.timedelta(seconds=1)
    return obj.replace(microsecond=0)
