package main

import (
	"runtime"
	"time"
)

func intersection(a []int, b []int) []int {
	c := []int{}

	for _, n := range a {
		for _, m := range b {
			if n == m {
				c = append(c, n)
                break
			}
		}
	}

	return c
}

func intersection2(a []int, b []int) []int {
	c := map[int]bool{}

    var smaller []int
    var larger []int

    if len(a) < len(b) {
        smaller = a
        larger = b
    } else {
        smaller = b
        larger = a
    }

    for _, n := range smaller {
        c[n] = true
    }

    intersection := []int{}

    for _, m := range larger {
        if (c[m]) {
            intersection = append(intersection, m)
        }
    }

	return intersection
}

func main() {
    maxIterations := 10000000

    a := []int{}
    b := []int{}

    for i := range maxIterations {
        a = append(a, i)
        b = append(b, i)
    }

    //intersectionStart := time.Now()
    //result := intersection(a, b)
    //intersectionEnd := time.Since(intersectionStart)

    intersection2Start := time.Now()
    result2 := intersection2(a, b)
    intersection2End := time.Since(intersection2Start)

    //println("Intersection result: ", len(result))
    println("Intersection2 result: ", len(result2))
    //println("Intersection took: ", intersectionEnd.Milliseconds())
    println("Intersection2 took: ", intersection2End.Milliseconds())

    var memoryStats runtime.MemStats
    runtime.ReadMemStats(&memoryStats)

    println("Memory Allocated: ", memoryStats.Alloc / 1024.0 / 1024.0, "MB")
}
