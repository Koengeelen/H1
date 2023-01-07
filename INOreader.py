import serial
import csv
import re
import datetime

ser = serial.Serial('/dev/ttyACM0' , 9600)

while True:
    now = datetime.datetime.now()
    date_time = now.strftime("%Y-%m-%d_%H-%M-%S")

    filename = f"{date_time}.csv"

    count = 0

    for i in range(100):
        data = ser.readline()
        data=data.decode("utf-8")
        list_data = data.split("    ")

        count+=1
        print(count)


        with open(filename, "a" , newline"\n") as csvfile:
            write = csv.writer(csvfile)
            write.writerow(list_data)

            