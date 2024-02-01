import matplotlib.pyplot as plt


file_path = "fake_log.txt"
with open(file_path, 'r') as file:
    lines = file.readlines()

# Initialize lists to store data for each graph
timestamps = []
cycles = []

dataset_names = [
    "Cycle Time",
    "Altitude",
    "Gyro",
    "Mag",
    "Acceleration",
    "Temperature"
]

datasets = [[] for _ in range(len(dataset_names))]
print(len(datasets))
for line in lines:
    values = line.strip().split(',')
    
    timestamps.append(float(values[0]))
    cycles.append(values[1])
    
    # datasets[0].append(int(values[0]) / int(values[1]))
    datasets[0].append(values[3])

# # Display legend
# lines, labels = ax1.get_legend_handles_labels()
# lines2, labels2 = ax2.get_legend_handles_labels()
# ax2.legend(lines + lines2, labels + labels2, loc='upper left')

for i in range(len(datasets)):
    fig, ax = plt.subplots(constrained_layout=True)

    ax.plot(timestamps, datasets[i])
    ax.set_title(dataset_names[i])

    ax.set_xlabel("Timestamps")
    ax.set_xticks(timestamps, labels=timestamps)

    # ax2 = ax.twiny()
    # ax2.set_xlabel("Cycles")
    # ax2.set_xticks(timestamps, labels=cycles)

    plt.show(block=False)

plt.show()
