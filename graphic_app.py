#from matplotlib import pyplot as plt
#import csv
#import glob


while True:

    #1
    # print(glob.glob("./*.csv"))
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

    if menu_indicator = 1:

        val1 = 0
        val2 = 1
        val3 = 2
        val4 = 3
        val5 = 10

        with open(f'{filename}.csv') as f:
            reader = csv.reader(f)
            data = list(reader)

        values1 = [floatrow(row[val1])]
        values2 = [floatrow(row[val2])]
        values3 = [floatrow(row[val3])]
        values4 = [floatrow(row[val4])]
        values5 = [floatrow(row[val5])]

        fig, ax = plt.subplots()

        ax.plot(values5, values1, color = 'blue' , linewidth = 2)
        ax.plot(values5, values2, color = 'red' , linewidth = 2)
        ax.plot(values5, values3, color = 'green' , linewidth = 2)
        ax.plot(values5, values4, color = 'yellow' , linewidth = 2)

        plt.show()




