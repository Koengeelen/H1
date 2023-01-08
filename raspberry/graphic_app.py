from matplotlib import pyplot as plt
import csv
import glob


while True:

    print(glob.glob("./*.csv"))
    filename = input("Enter file to graph(Y-M-D_H-M-S)")

    print("Menu:")
    print("1. --Temperatures--")
    print("2. ----Pressures---")
    print("3. ------Flow------")
    print("4. ------RPM-------")
    print("5. ----Throttle----")
    print("6. ---Fuel level---")
    print("7. -----Custom-----")
    print("   ----------------")

    menu_indicator = input("Input data to graph:")

    if menu_indicator == "1":   #TEMPERATURE (4)

        val1 = 0
        val2 = 1
        val3 = 2
        val4 = 3
        val5 = 10

        with open(f'{filename}.csv') as f:
            reader = csv.reader(f)
            data = list(reader)

        values1 = [float(row[val1]) for row in data]
        values2 = [float(row[val2]) for row in data]
        values3 = [float(row[val3]) for row in data]
        values4 = [float(row[val4]) for row in data]
        values5 = [float(row[val5]) for row in data]

        fig, ax = plt.subplots()

        ax.plot(values5, values1, color = 'blue' , linewidth = 2)
        ax.plot(values5, values2, color = 'red' , linewidth = 2)
        ax.plot(values5, values3, color = 'green' , linewidth = 2)
        ax.plot(values5, values4, color = 'yellow' , linewidth = 2)

        plt.show()


    if menu_indicator == "2":   #PRESSURE (2)

        val1 = 4
        val2 = 5
        val3 = 10

        with open(f'{filename}.csv') as f:
            reader = csv.reader(f)
            data = list(reader)

        values1 = [float(row[val1]) for row in data]
        values2 = [float(row[val2]) for row in data]
        values3 = [float(row[val3]) for row in data]

        fig, ax = plt.subplots()

        ax.plot(values3, values1, color = 'blue' , linewidth = 2)
        ax.plot(values3, values2, color = 'red' , linewidth = 2)

        plt.show()

    if menu_indicator == "3":    #FLOW

        val1 = 6
        val2 = 10

        with open(f'{filename}.csv') as f:
            reader = csv.reader(f)
            data = list(reader)

        values1 = [float(row[val1]) for row in data]
        values2 = [float(row[val2]) for row in data]

        fig, ax = plt.subplots()

        ax.plot(values2, values1, color = 'blue' , linewidth = 2)

        plt.show()


    if menu_indicator == "4":    #RPM

        val1 = 7
        val2 = 10

        with open(f'{filename}.csv') as f:
            reader = csv.reader(f)
            data = list(reader)

        values1 = [float(row[val1]) for row in data]
        values2 = [float(row[val2]) for row in data]

        fig, ax = plt.subplots()

        ax.plot(values2, values1, color = 'blue' , linewidth = 2)

        plt.show()


    if menu_indicator == "5":    #THROTTLE

        val1 = 8
        val2 = 10

        with open(f'{filename}.csv') as f:
            reader = csv.reader(f)
            data = list(reader)

        values1 = [float(row[val1]) for row in data]
        values2 = [float(row[val2]) for row in data]

        fig, ax = plt.subplots()

        ax.plot(values2, values1, color = 'blue' , linewidth = 2)

        plt.show()


    if menu_indicator == "6":    #FUEL

        val1 = 9
        val2 = 10

        with open(f'{filename}.csv') as f:
            reader = csv.reader(f)
            data = list(reader)

        values1 = [float(row[val1]) for row in data]
        values2 = [float(row[val2]) for row in data]

        fig, ax = plt.subplots()

        ax.plot(values2, values1, color = 'blue' , linewidth = 2)

        plt.show()


    if menu_indicator == "7":    #CUSTOM

        number_of_values = int(input("Select number of values to compare"))   #preguntar por el número de variables para los bucles

        variables = {}                                                        #diccionario para cada variable con su valor
        principal_value = int(input("Select principal value:"))               #guardar variable principal a la que comparar todos los datos

        for i in range(number_of_values-1):
            variables[f'val{i}'] = int(input(f'Select value {i+1}:'))         #Guardar cada variable con el valor deseado

        with open(f'{filename}.csv') as f:                                    #Abrir y preparar lectura de datos
            reader = csv.reader(f)
            data = list(reader)

        values_list = {}
        for i in range(number_of_values-1):
            values_list[f'values{i}'] = [float(row[f'val{i}']) for row in data]                         #Guardar los valores del documento en cada variable

        fig, ax = plt.subplots()

        list_of_colors = ["red", "blue", "green", "orange", "yellow", "purple", "pink", "black"]        #Lista para variar los colores de las gráficas

        for i in range(number_of_values-1):

            ax.plot(principal_value, f'values{i}', color = list_of_colors[i] , linewidth = 2)           #Imprimir cada gráfica

        plt.show()


        



