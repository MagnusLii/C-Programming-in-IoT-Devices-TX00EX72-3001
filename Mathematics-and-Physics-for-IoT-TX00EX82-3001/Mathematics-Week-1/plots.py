import matplotlib.pyplot as plt

# Function to generate the sequence
def generate_sequence(n):
    x = [1]
    for k in range(1, n):
        x.append(x[k-1] / 2)
    return x

# Generate the first five terms of the sequence
n_terms = 5
sequence = generate_sequence(n_terms)

# Plotting the graph
plt.plot(range(n_terms), sequence, marker='o', linestyle='-', color='b')
plt.title('Recursive Sequence: $x[k] = \\frac{x[k-1]}{2}$, $x[0]=1$')
plt.xlabel('k')
plt.ylabel('$x[k]$')
plt.grid(True)
plt.show()
