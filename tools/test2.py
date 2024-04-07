import matplotlib.pyplot as plt

# Read data from the text file
with open('log_apogee.txt', 'r') as file:
    lines = file.readlines()

# Extract values and calculate the ratio
x_values = []
y_values = []

for line in lines:
    values = line.strip().split(',')
    x_values.append(float(values[0]))
    y_values.append(float(values[1]))

ratios = [x / y for x, y in zip(x_values, y_values)]

# Plot the chart
plt.plot(ratios, marker='o')
plt.xlabel('Line Number')
plt.ylabel('Value1 / Value2 Ratio')
plt.title('Ratio of Value1 to Value2 for Each Line')
plt.show()
