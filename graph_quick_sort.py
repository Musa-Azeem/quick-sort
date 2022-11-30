#!/usr/bin/env python3

import pandas as pd
import seaborn as sns
import math
import sys

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: ./graph_quick_sort.py [Average Times File]")
        exit(1)
    filename = sys.argv[1]

    df = pd.read_csv(filename, sep='    ')
    print(df)

    # Plot Average Execution Time
    plot = sns.lineplot(data=df, x='Input Size', y='Average Execution Time (ms)')
    plot.set_xticks(range(0, 1001, 100))
    plot.set_yticks(range(0, math.ceil(max(df['Average Execution Time (ms)']))+10, 10))
    plot.set_xlabel('Input Size')
    plot.set_ylabel('Average Execution Time (ms)')
    plot.set_title('Average Execution Time for Quick Sort')
    plot.get_figure().savefig('Azeem_Musa_plotAverageExecutionTime.jpg')