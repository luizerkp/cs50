-- SELECT A.name, B.account_number  FROM people A
-- JOIN bank_accounts B ON A.id = B.person_id
-- WHERE B.account_number IN (28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199);
--  SELECT caller, receiver, duration FROM phone_calls WHERE year = 2020 AND day = 28 AND month = 7 AND duration < 60;
SELECT A.id, origin_airport.city, destination_airport.city, A.hour, A.minute FROM flights A
JOIN airports origin_airport ON A.origin_airport_id = origin_airport.id 
JOIN airports destination_airport ON A.destination_airport_id = destination_airport.id 
WHERE origin_airport.city = "Fiftyville" AND A.day = 29 AND A.month = 7 AND A.year = 2020
ORDER BY hour, minute;