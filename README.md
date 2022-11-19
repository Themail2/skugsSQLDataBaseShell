# skugsSQLDataBaseShell
contains functions to interact with the SQL database for skugs replays
# NOTE PLEASE READ
You will have to configure the SQL cursor to work with your database, my database is not the same as yours
You can find the instaciation of the SQL curson on line 66 in AddDatabaseEntrySrcript.py
# DO NOT CHANGE THE DRIVER
Youll probably only need to change the password and database name, please check all of the cursor.execute() lines to make sure they match your database arguments.

# AutomatedSkugsReplay is the HTTP Server
Please also check the SQL in that project as well, make sure the cursor.execute() lines are formatted properly for your database and that the cursor is configured to login to your database.

hope this helps

---Themail---
