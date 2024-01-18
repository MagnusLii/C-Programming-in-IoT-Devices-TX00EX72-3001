import matplotlib.pyplot as plt

# 3a
def generate_sequence(n):
    x = [1]
    for k in range(1, n):
        x.append(x[k-1] / 2)
    return x

n_terms = 5
sequence = generate_sequence(n_terms)

plt.plot(range(n_terms), sequence, marker='o', linestyle='-', color='b')
plt.title('Recursive Sequence: $x[k] = \\frac{x[k-1]}{2}$, $x[0]=1$')
plt.xlabel('k')
plt.ylabel('$x[k]$')
plt.grid(True)
plt.show()

# 3b
def generate_sequence(n):
    x = [2, 1]
    for k in range(2, n):
        x.append(3 * x[k-1] - 2 * x[k-2])
    return x

n_terms = 5
sequence = generate_sequence(n_terms)

plt.plot(range(n_terms), sequence, marker='o', linestyle='-', color='b')
plt.title('Recursive Sequence: $x[k] = 3x[k-1] - 2x[k-2]$, $x[0]=2$, $x[1]=1$')
plt.xlabel('k')
plt.ylabel('$x[k]$')
plt.grid(True)
plt.show()
