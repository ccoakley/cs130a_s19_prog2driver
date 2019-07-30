import random


class ProblemGenerator(object):
    def __init__(self, initial_size, follow_up_size):
        self.initial_size = initial_size
        self.follow_up_size = follow_up_size

    def run(self):
        print(self.initial_size)
        self._generate_dists()
        rows = self._generate_upper_diagonal()
        self._print_dense_graph(rows)
        print()
        new_edges = self._generate_follow_up()
        self._print_follow_up(new_edges)
        print()

    def _generate_follow_up(self):
        pairs = [(i,j) for i in range(0, self.initial_size-1) for j in range(i+1, self.initial_size)]
        selected = random.sample(pairs, self.follow_up_size)
        return [(i,j,self.dists.pop()) for (i,j) in selected]

    def _generate_follow_up_bad(self):
        """
        Generates a list of new edges (from, to, cost)
        """
        edges = []
        for i in range(0, self.follow_up_size):
            name = i + self.initial_size
            # at least one connection
            selection = sorted(random.sample(
                range(0, self.initial_size),
                random.choice(range(1, self.initial_size+1))
            ))
            edges.extend([(item, name, self.dists.pop()) for item in selection])
        return edges

    def _print_follow_up(self, edges):
        for edge in edges:
            print("{} {} {}".format(*edge))


    def _generate_dists(self):
        """
        Generates a random list of unique distances.
        Guaranteed to be large enough for the problem.
        """
        list_size = self.initial_size**2 + self.follow_up_size
        self.dists = list(range(1, list_size+1))
        random.shuffle(self.dists)

    def _generate_upper_diagonal(self):
        rows = []
        for i in range(0, self.initial_size):
            offset = self.initial_size*i+1
            row = self.dists[offset:offset+self.initial_size-i]
            rows.append(row)
        return rows

    def _print_dense_graph(self, rows):
        for i in range(0, self.initial_size):
            printable = []
            for j in range(0, self.initial_size):
                if i<j:
                    printable.append(rows[i][j-i])
                if i==j:
                    printable.append(0)
                if i>j:
                    printable.append(rows[j][i-j])
            print(" ".join(map(str, printable)))

def main():
    pg = ProblemGenerator(10, 10)
    pg.run()

if __name__ == "__main__":
    main()
