-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Entry # 1
-- First step is to open the db
sqlite3 fiftyville.db

-- Entry # 2
-- To start off I need to understand what tables are in the database and what kind of info (i.e. columns) those tables contain
-- It is important to note which tables are joining tables
.tables
-- tables included are:
    -- airports                  crime_scene_reports       people
    -- atm_transactions          flights                   phone_calls
    -- bank_accounts             interviews
    -- courthouse_security_logs  passengers

--Entry # 3
.schema
-- The info on the tables are:
    -- airports:            -- crime_scene_reports:         -- people:
        -- id INTEGER,              -- id INTEGER,              -- id INTEGER,
        -- abbreviation TEXT,       -- year INTEGER,            -- name TEXT,
        -- full_name TEXT,          -- month INTEGER,           -- phone_number TEXT,
        -- city TEXT,               -- day INTEGER,             -- passport_number INTEGER,
        -- PRIMARY KEY(id)          -- street TEXT,             -- license_plate TEXT,
                                    -- description TEXT,        -- PRIMARY KEY(id)
                                    -- PRIMARY KEY(id)

    -- atm_transactions:        -- flights:                         -- phone_calls:
        -- id INTEGER,              -- id INTEGER,                      -- id INTEGER,
        -- account_number INTEGER,  -- origin_airport_id INTEGER,       -- caller TEXT,
        -- year INTEGER,            -- destination_airport_id INTEGER,  -- receiver TEXT,
        -- month INTEGER,           -- year INTEGER,                    -- year INTEGER,
        -- day INTEGER,             -- month INTEGER,                   -- month INTEGER,
        -- atm_location TEXT,       -- day INTEGER,                     -- day INTEGER,
        -- transaction_type TEXT,   -- hour INTEGER,                    -- duration INTEGER,
        -- amount INTEGER,          -- minute INTEGER,                  -- PRIMARY KEY(id)
        -- PRIMARY KEY(id)          -- PRIMARY KEY(id),
                                    -- FOREIGN KEY(origin_airport_id)
                                        --REFERENCES airports(id),
                                    -- FOREIGN KEY(destination_airport_id)
                                        --REFERENCES airports(id)

    -- bank_accounts:               -- interviews:      -- courthouse_security_logs: -- passengers
        -- account_number INTEGER,      -- id INTEGER,      -- id INTEGER,              -- flight_id INTEGER,
        -- person_id INTEGER,           -- name TEXT,       -- year INTEGER,            -- passport_number INTEGER,
        -- creation_year INTEGER,       -- year INTEGER,    -- month INTEGER,           -- seat TEXT
        -- FOREIGN KEY(person_id)       -- month INTEGER,   -- day INTEGER,             -- FOREIGN KEY(flight_id)
            --REFERENCES people(id)     -- day INTEGER,     -- hour INTEGER,                -- REFERENCES flights(id)
                                        -- transcript TEXT, -- minute INTEGER,
                                        -- PRIMARY KEY(id)  -- activity TEXT,
                                                            -- license_plate TEXT,
                                                            -- PRIMARY KEY(id)

--Entry # 4
    -- **All you know is that the theft took place on July 28, 2020 and that it took place on Chamberlin Street.**
    -- Lets read the description of the crime scene reports for 7/28/2020 on Chamberlin Street
SELECT description FROM crime_scene_reports WHERE year = 2020 AND day = 28 AND month = 7;
    --NOTES:
        -- CS50 duck stolen TIME: 10:15 LOCATION: Chamberlin Street courthouse -- 3 witnesses interviews conducted.
            -- witnesses' transcrits mention courthouse
            -- on the same day crimes reported --TIME:
                --shoplifting                       --03:01
                --vandalism                         --12:04
                --money laundering                  --20:30
                --littering                         --16:36

--Entry # 5
    -- we should make note of the crimescene report id for the CS50 duck theft for easier cross-referencing
SELECT id, description FROM crime_scene_reports WHERE year = 2020 AND day = 28 AND month = 7;
    -- NOTES crime report id for CS50 duck theft is 295
            -- ***Not usefull for x-reference at this time.****

--Entry # 6
    -- Now we should review the witness interviews and see what about the courthouse stuck in their minds
SELECT name, transcript FROM interviews WHERE year = 2020 AND day = 28 AND month = 7 AND transcript LIKE "%courthouse%";
    --INTERVIEW NOTES: ** mentioning courthouse
        --RUTH: ~10 mins from theft saw thief get on a car and drive off from courthouse parking lot
                    -- Note to self: corroborate with courthouse_security_logs & prople
        --EUGENE: recognized thief from morning of crime, BEFORE ariving at courthouse
                --saw thief at ATM on Fifer Street withdrawing money
                    -- Note to self: corroborate with courthouse_security_logs, atm_transactions
                        -- bank_accounts and people
        --RAYMOND: saw thief call someone after leaving courthouse for less than a minute
                -- heard thief say planning to take earliest flight out of Fiftyvile on 7/29/2020
                -- also heard thief ask the other person to purchase ticket
                    --Note to self: corroborate with people, phone_calls & flights

--Entry # 7
    -- I need to find Eugene's license plate to find out whe she arrived at the court house
SELECT name, license_plate FROM people WHERE name like "eugene";
        -- Eugene's license plate is 47592FJ
            --***UPON FURTHER REVIEW THIS IS NOT NEED AS SHE MENTIONS THAT SHE WAS WALKING ****

--Entry # 8
    -- I need to check out the courthouse security logs for who exited with 10 minutes of theft to corroborate Ruth's witness statement
SELECT hour, minute, activity, license_plate FROM courthouse_security_logs
WHERE activity = "exit" AND year = 2020 AND day = 28 AND month = 7 AND hour = 10
AND minute >= 15 AND minute <= 25 ORDER BY hour, minute;
        --NOTES: The license numbers of the cars that left the court house within 10 minutes of theft

            -- hour| minute | activity  | license_plate
            -- 10  | 16     | exit      | 5P2BI95
            -- 10  | 18     | exit      | 94KL13X
            -- 10  | 18     | exit      | 6P58WS2
            -- 10  | 19     | exit      | 4328GD8
            -- 10  | 20     | exit      | G412CB7
            -- 10  | 21     | exit      | L93JTIZ
            -- 10  | 23     | exit      | 322W7JE
            -- 10  | 23     | exit      | 0NTHK55

--Entry # 9
    -- now I need to put names to those license plates to build a
    -- list of suspects seen leaving the scense between 10:15 and 10:25
SELECT license_plate, name, passport_number FROM people
WHERE license_plate IN ("5P2BI95", "94KL13X", "6P58WS2", "4328GD8", "G412CB7", "L93JTIZ", "322W7JE", "0NTHK55", "1106N58");
    --NOTES: by running the above query on all license plates leaving within 10 mins of the results are as follow
        -- plate#:   name:      passport#:
        -- 5P2BI95 | Patrick    2963008352
        -- 6P58WS2 | Amber      7526138472
        -- L93JTIZ | Elizabeth  7049073643
        -- G412CB7 | Roger      1695452385
        -- 4328GD8 | Danielle   8496433585
        -- 322W7JE | Russell    3592750733
        -- 0NTHK55 | Evelyn     8294398571
        -- 94KL13X | Ernest     5773159633

--Entry # 10
    -- Now to Corroborate Eugene's statement and see if any of the suspects
    -- whidrew money from ATM on Fifer Street the day of the theft
SELECT transaction_type, account_number FROM atm_transactions
WHERE month = 7 AND year = 2020 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type LIKE "%WITHDRAW%";
    --NOTES:
        -- TYPE:    ACCOUNT #:
        -- withdraw | 28500762
        -- withdraw | 28296815
        -- withdraw | 76054385
        -- withdraw | 49610011
        -- withdraw | 16153065
        -- withdraw | 25506511
        -- withdraw | 81061156
        -- withdraw | 26013199

--Entry # 11
    -- NOW to put a name to each account number in question
SELECT A.name, B.account_number FROM people A
JOIN bank_accounts B ON A.id = B.person_id
WHERE B.account_number IN (28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199);
    -- NOTES:
        -- name:        | account_number:
        -- Ernest       | 49610011  **
        -- Russell      | 26013199  **
        -- Roy          | 16153065
        -- Bobby        | 28296815
        -- Elizabeth    | 25506511  **
        -- Danielle     | 28500762  **
        -- Madison      | 76054385
        -- Victoria     | 81061156
            -- ** Indicates that the person left the courthouse within 10 minutes of the Theft. See entry 6 RUTH's statement

    -- New Suspect list
        -- name         | account#  | passport#
        -- Ernest       | 49610011  | 5773159633
        -- Russell      | 26013199  | 3592750733
        -- Elizabeth    | 25506511  | 7049073643
        -- Danielle     | 28500762  | 8496433585

--Entry # 12
    -- Now I need to see if any of the suspects were on the earliest flight out fiftyville on 7/29/2020
    -- First I need to find out the flight info on this flight
SELECT A.id, origin_airport.city, destination_airport.city, A.hour, A.minute FROM flights A
JOIN airports origin_airport ON A.origin_airport_id = origin_airport.id
JOIN airports destination_airport ON A.destination_airport_id = destination_airport.id
WHERE origin_airport.city = "Fiftyville" AND A.day = 29 AND A.month = 7 AND A.year = 2020
ORDER BY hour, minute;
        --NOTES: earliest flight is at 08:20 and it's id is 36
            -- id | city       | city          | hour  | minute
            -- 36 | Fiftyville | London        | 8     | 20 ** see entry 6: RAYMOND witness statement
            -- 43 | Fiftyville | Chicago       | 9     | 30
            -- 23 | Fiftyville | San Francisco | 12    | 15
            -- 53 | Fiftyville | Tokyo         | 15    | 20
            -- 18 | Fiftyville | Boston        | 16    | 0

--Entry # 13
    --Let's see if any of our suspects is on that flight
SELECT seat, passport_number FROM passengers
JOIN flights ON flights.id = passengers.flight_id
WHERE flights.id = 36 AND passengers.passport_number IN (5773159633, 3592750733, 7049073643, 8496433585, 1988161715);
    -- NOTES:
        -- seat    | passport_number   | name
        -- 4A      | 5773159633        | Ernest
        -- 7B      | 8496433585        | Danielle

--Entry # 14
    -- Now lets see if any of the remaining suspects made a call
    -- that lasted for less than a minute within 10 minutes of the theft
    -- To do this I need to make note of the new suspect list phone numbers
SELECT name, phone_number FROM people WHERE name IN ("Ernest", "Danielle");
    --NOTES:
        -- name        | phone_number
        -- Danielle    | (389) 555-5198
        -- Ernest      | (367) 555-5533

--Entry # 15
    -- Now I need query the phonecalls
SELECT caller, receiver, duration FROM phone_calls
WHERE year = 2020 AND day = 28 AND month = 7 AND duration < 60 AND caller IN ("(389) 555-5198", "(367) 555-5533");
    --Notes: Ernest is our culprit
        -- name        |caller          | receiver          | duration
        -- Ernest      | (367) 555-5533 | (375) 555-8161    | 45

--Entry # 16
    --Now to find Ernest's accomplice we should find the name of the person he called with phone# (676) 555-6554
SELECT name FROM people WHERE phone_number = "(375) 555-8161";
    --NOTES:
        -- The accomplice is Berthold

--  ******CONCLUSIONS:
                    -- Based on the eveidence gathered the Thief is most likely Ernest as
                        -- he is the only suspect that matches all of the following:
                            -- withdrew money from the ATM on FIFER street see entries 6, 10, & 11
                            -- left the courthouse parking lot within 10mins of the theft see entries 6, 8, & 9
                            -- is on the first flight out of fiftyville see entries 6, 9, 12, & 13
                            -- placed a phone call that lasted less than a minute 6, 14, & 15

                    -- The accomplice is most likely Berthold based on the fact that he is the person Ernest called
                        -- for for less than a minute on the day of theft see entry 6 & 15

                    -- The destination the thief fled to is London see entry 6 & 12

