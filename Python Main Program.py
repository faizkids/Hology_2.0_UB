from http.server import HTTPServer, BaseHTTPRequestHandler
import mysql.connector
import time

mydb = mysql.connector.connect(
  host="127.0.0.1",
  user="root",
  passwd="",
  database="hology2"
)

mycursor = mydb.cursor()

print("Server Menyala!")

class Serv(BaseHTTPRequestHandler):

    def do_GET(self):
        data=self.path
        data_fix = self.path[1:]

        print(len(data_fix))
        if(len(data_fix)==24):
            print("Data = " + data_fix)
            data1 = data_fix[3:6]
            data2 = data_fix[13]
            data3 = data_fix[21:]
            print(data1)
            print(data2)
            print(data3)
            sql = "INSERT INTO bank_sampah (id_user,jenis, berat) VALUES (%s, %s, %s)"
            val = (data1,data2,data3)
            mycursor.execute(sql, val)
            mydb.commit()
            print("Data Sukses Terkirim")

        elif(len(data_fix)==23):
            print("Data = " + data_fix)
            data1 = data_fix[3:5]
            data2 = data_fix[12]
            data3 = data_fix[20:]
            print(data1)
            print(data2)
            print(data3)
            sql = "INSERT INTO bank_sampah (id_user,jenis, berat) VALUES (%s, %s, %s)"
            val = (data1,data2,data3)
            mycursor.execute(sql, val)
            mydb.commit()
            print("Data Sukses Terkirim")

        elif(len(data_fix)==22):
            print("Data = " + data_fix)
            data1 = data_fix[3:4]
            data2 = data_fix[11]
            data3 = data_fix[19:]
            print(data1)
            print(data2)
            print(data3)
            sql = "INSERT INTO bank_sampah (id_user,jenis, berat) VALUES (%s, %s, %s)"
            val = (data1,data2,data3)
            mycursor.execute(sql, val)
            mydb.commit()
            print("Data Sukses Terkirim")

        
        try:
            file_to_open = index.html
            self.send_response(200)
        except:
            file_to_open = "Data Terkirim"
            self.send_response(404)
        self.end_headers()
        self.wfile.write(bytes(file_to_open, 'utf-8'))


##httpd = HTTPServer(('127.0.0.1', 8080), Serv)
httpd = HTTPServer(('0.0.0.0', 8090), Serv)
httpd.serve_forever()



## Format = http://localhost:8090/jenis=1&berat=3.2
##          http://localhost:8090/id=012&jenis=1&berat=3.2
##http://10.212.174.123:8090/id=012&jenis=1&berat=3.2

