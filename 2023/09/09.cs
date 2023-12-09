class M {
    private static bool allZero(List<int> s) {
        foreach (var i in s) {
            if (i != 0)
                return false;
        }

        return true;
    }

    public static void Main(string[] args) {
        var p1 = 0;
        var p2 = 0;

        foreach (var line in File.ReadAllText(args[0]).Split("\n")) {
            if (line == "")
                continue;

            var seqs = new List<List<int>>();
            seqs.Add(line.Split(" ").ToList().ConvertAll(int.Parse));
            var seq = seqs[0];

            while (!allZero(seq)) {
                var next = new List<int>();

                for (var i = 0; i < seq.Count - 1; i++) {
                    next.Add(seq[i + 1] - seq[i]);
                }

                seqs.Add(next);
                seq = next;
            }

            var prev = 0;
            for (var i = seqs.Count - 1; i > 0; i--) {
                p1 += seqs[i - 1][seqs[i - 1].Count - 1];
                prev = seqs[i - 1][0] - prev;
            }

            p2 += prev;
        }

        Console.WriteLine(p1);
        Console.WriteLine(p2);
    }
}
