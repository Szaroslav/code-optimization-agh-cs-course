import sys
import matplotlib.pyplot as plt
import numpy as np

# Indicate the number of floating point operations that can be executed
# per clock cycle
nflops_per_cycle = 16

# Indicate the number of processors being used (in case you are using a
# multicore or SMP)
nprocessors = 1

# Indicate the clock speed of the processor.  On a Linux machine this info
# can be found in the file /proc/cpuinfo
#
# Note: some processors have a "turbo boost" mode, which increases
# the peak clock rate...
#
GHz_of_processor = 3.22


class Parser:
    def __init__(self, file_name) -> None:
        self.attrs = {}
        with open(file_name) as file:
            self.toks = file.read().split()
            self.toksi = 0
            file.close()
            self.attrs = self.parse()

    def next(self):
        tok = self.toks[self.toksi]
        self.toksi += 1
        return tok

    def get_var_name(self):
        return self.next()

    def get_symbol(self, sym):
        tok = self.next()
        assert(tok == sym)
        return tok

    def get_value(self):
        value = None
        tok = self.next()
        if tok == '[':
            # list
            value = []
            tok = self.next()
            while not tok.startswith(']'):
                value.append(float(tok))
                tok = self.next()
        elif tok.startswith("'"):
            value = tok[1:-2]

        assert value != None
        return value

    def parse(self):
        res = {}
        while self.toksi < len(self.toks):
            var = self.get_var_name()
            self.get_symbol('=')
            val = self.get_value()
            res[var] = val
        return res

    def __getattr__(self, name):
        return self.attrs[name]


if __name__ == "__main__":
    FILENAMES = [
        "chol1",
        "chol2",
        "chol3",
        "chol4",
        "chol5",
        "chol6",
        "chol7",
    ]

    title = "Porównanie faktoryzacji Choleskiego"
    filenames = FILENAMES

    max_gflops = nflops_per_cycle * nprocessors * GHz_of_processor;

    fig, ax = plt.subplots()

    for filename in filenames:
        results = Parser(f"output_{filename}.txt")
        data = np.array(results.chol).reshape(-1, 3)
        x_limits = (data[0, 0], data[-1, 0])
        ax.plot(data[:, 0], data[:, 1], label=results.version, marker=".")

    ax.set(xlabel="Rozmiar macierzy", ylabel="Przybliżone GFLOPS",
        title=title)
    ax.grid()
    ax.legend(fontsize=8, loc="center right", bbox_to_anchor=(1.315, 0.5))
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.85, box.height])

    ax.set_xlim(x_limits)
    ax.set_ylim([0, max_gflops])

    fig.savefig("result-chart.png")
    plt.show()
