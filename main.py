import time

def itersection2(a: list[int], b: list[int]):
    smaller = a if len(a) < len(b) else b
    larger = b if len(a) < len(b) else a

    c = {}

    for n in smaller:
        c[n] = True

    intersection = []

    for m in larger:
        if c[m]:
            intersection.append(m)

    return intersection

def main():
    max_interaction = 100000000

    a = []
    b = []

    for i in range(max_interaction):
        a.append(i)
        b.append(i)

    # Time execution of intersection2
    start = time.time()
    intersection = itersection2(a, b)
    end = time.time()

    ellapsed_ms = (end - start) * 1000
    print(f"Execution time of itersection2: {ellapsed_ms:.2f} ms")
    print(len(intersection))

if __name__ == "__main__":
    main()
