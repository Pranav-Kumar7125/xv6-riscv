import pandas as pd
import matplotlib.pyplot as plt

# Read data
df = pd.read_csv('syscall_plot.csv')

# Create plot
plt.figure(figsize=(12, 6))
plt.plot(df['Minute'], df['Calls'], marker='o')
plt.title('System Calls per Minute')
plt.xlabel('Minute')
plt.ylabel('Number of Calls')
plt.grid(True)
plt.savefig('syscall_stats.png')
plt.show()

