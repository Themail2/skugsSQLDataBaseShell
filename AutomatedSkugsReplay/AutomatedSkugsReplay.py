from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import pyodbc
import vgamepad as vg



hostName = "localhost"
serverPort = 8080
currentIndex = 0
unresolvedReplaySubmissionNumbers = []
steamIDs = []
unresolvedReplayRNDData = []
playerNames = []

def getUnsolvedData(cursor):
    unresolvedReplayData = []
    cursor.execute('SELECT * FROM replaydatabase.replaymessages WHERE resolved = 0;')
    for x in cursor:
        unresolvedReplayData.append(x)
    return unresolvedReplayData
#CURL REQUEST CMD COMMAND 
#curl --location --request GET localhost:8080 --form winner="Player1"
class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        try:
            content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
            post_data = self.rfile.read(content_length) # <--- Gets the data itself
            self.send_response(200)
            self.send_header("Content-type", "text/plain")
            self.end_headers()
            self.wfile.write(bytes("True", "utf-8"))
            post_data = post_data.decode('utf-8')
            if not(post_data.find("Player1") == -1):
                print("Player 1 won")
                currentCommand = "UPDATE replaydatabase.replaymessages SET resolved = 1, winner = '" + unresolvedReplayData[currentIndex][1] + "' WHERE submission = " + str(unresolvedReplayData[currentIndex][6]) + " AND id ='" + unresolvedReplayData[currentIndex][5] + "';"
                print(currentCommand)
                cursor.execute(currentCommand)
                connection.commit()
                
            if not(post_data.find("Player2") == -1):
                print("player2wins")
                currentCommand = "UPDATE replaydatabase.replaymessages SET resolved = 1 , winner = '" + str(unresolvedReplayData[currentIndex][6]) + "'WHERE submission = " + str(unresolvedReplaySubmissionNumbers[currentIndex]) + " AND id ='" + unresolvedReplayData[currentIndex][5] + "';"
                print(currentCommand)
                cursor.execute(currentCommand)
                connection.commit()
                

            

        except Exception as err:
            print(err)
            self.wfile.write("POST request for {}".format(self.path).encode('utf-8'))
            self.send_response(500)
            self.send_header("Content-type", "text/plain")
            self.end_headers()
            self.wfile.write(bytes("False", "utf-8"))
    def do_POST(self):
        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data = self.rfile.read(content_length) # <--- Gets the data itself
        print("POST request,\nPath: %s\nHeaders:\n%s\n\nBody:\n%s\n",
                str(self.path), str(self.headers), post_data.decode('utf-8'))

        self._set_response()
        self.wfile.write("POST request for {}".format(self.path).encode('utf-8'))
if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    connection = pyodbc.connect('driver=MySQL ODBC 8.0 Unicode Driver', host='localhost', database='replaydatabase', user='root', password='PooperScooper69')
    cursor = connection.cursor()
    try:
        #Instanciates a virtual controller object to naviagate the replay menus
        #Fetch unresolved entries in SQL DataBase before the main program loop
        #gets list of rows as lists (lists in lists)
        unresolvedReplayData = getUnsolvedData(cursor)
        print("Done...\n")

        print("Server started http://%s:%s" % (hostName, serverPort))
        #main program loop
        webServer.handle_request()
        print("Server stopped for database writing...")

    except KeyboardInterrupt:
        pass
    
    webServer.server_close()
    print("Server stopped.")
