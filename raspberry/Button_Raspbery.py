
from matplotlib import pyplot as plt
import glob
from enum import Enum
from pick import pick
import serial
import csv
import re
import datetime
import time
import RPi.GPIO as GPIO 

ser = serial.Serial('/dev/ttyACM0' , 9600)

GPIO.setmode(GPIO.BCM)
GPIO.setup(10, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

class MenuOption(Enum):
    TEMPERATURE = 0
    PRESSURE = 1
    FLOW = 2
    RPM = 3
    THROTTLE = 4
    FUEL = 5
    POWER = 6
    CUSTOM = 7


class Headers(Enum):
    TEMP1 = 0
    TEMP2 = 1
    TEMP3 = 2
    TEMP4 = 3
    PRESS1 = 4
    PRESS2 = 5
    FLOW1 = 6
    RPM = 7
    THROTTLE = 8
    FUEL = 9
    TIME = 10
    POWER = 11


colors = [
    "red",
    "blue",
    "green",
    "orange",
    "yellow",
    "purple",
    "pink",
    "black",
    "gray",
    "brown"
]


def show_file_selector():
    title = "Choose source file:"
    options = glob.glob("./*.csv")
    option, _ = pick(options, title, indicator="=>")
    return option


def show_menu():
    title = "Input data to graph:"
    options = ["Temperatures", "Pressures", "Flow", "RPM", "Throttle", "Fuel level", "Power", "Custom"]
    _, index = pick(options, title, indicator="=>")
    return index


def main():
    filename = show_file_selector()
    menu_indicator = show_menu()
    graph(menu_indicator, filename)


def plot(x, values):
    color = 0
    _, ax = plt.subplots()
    for value in values:
        ax.plot(x, value, color=colors[color], linewidth=2)
        color += 1
    plt.show()


def graph(menu_indicator, filename):



    input_state = GPIO.input(10)
    if input_state:
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
        
        #Code to read form the arduino and save a document as a csv

    else:
    
    with open(f"{filename}") as f:
        reader = csv.reader(f)
        data = list(reader)

        x = [float(row[Headers.TIME.value]) for row in data]

        match menu_indicator:
            case MenuOption.TEMPERATURE.value:
                values = [
                    [float(row[Headers.TEMP1.value]) for row in data],
                    [float(row[Headers.TEMP2.value]) for row in data],
                    [float(row[Headers.TEMP3.value]) for row in data],
                    [float(row[Headers.TEMP4.value]) for row in data],
                ]

            case MenuOption.PRESSURE.value:
                values = [
                    [float(row[Headers.PRESS1.value]) for row in data],
                    [float(row[Headers.PRESS2.value]) for row in data],
                ]

            case MenuOption.FLOW.value:
                values = [[float(row[Headers.FLOW1.value]) for row in data]]

            case MenuOption.RPM.value:
                values = [[float(row[Headers.RPM.value]) for row in data]]

            case MenuOption.THROTTLE.value:
                values = [[float(row[Headers.THROTTLE.value]) for row in data]]

            case MenuOption.FUEL.value:
                values = [[float(row[Headers.FUEL.value]) for row in data]]

            case MenuOption.POWER.value:
                values = [[float(row[Headers.POWER.value]) for row in data]]

            case MenuOption.CUSTOM.value:

                title = "Select main data to compare to:"
                options = [
                    "TEMP1", "TEMP2", "TEMP3", "TEMP4",
                    "PRESS1", "PRESS2", "FLOW1", "RPM",
                    "THROTTLE", "FUEL", "TIME", "POWER"
                ]
                _, index = pick(options, title, indicator="=>")
                x = [float(row[index]) for row in data]

                title = 'Select additional data to compare: '
                selected = pick(options, title, multiselect=True, min_selection_count=1)

                values = []
                for s in selected:
                    values.append([float(row[s[1]]) for row in data])

        plot(x, values)

while True:
    
    if __name__ == "__main__":

        # while True:
    main()

