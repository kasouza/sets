function intersection(a, b) {
  const c = []

  outer:
  for (let i = 0; i < a.length; i++) {
    for (let j = 0; j < b.length; j++) {
      if (a[i] === b[j]) {
        c.push(a[i])
        continue outer;
      }
    }
  }

  return c
}

function intersection2(a, b) {
  const c = {}

  const smaller = a.length < b.length ? a : b
  const larger = a.length < b.length ? b : a

  for (let i = 0; i < smaller.length; i++) {
    c[smaller[i]] = true
  }

  const intersection = []

  for (let j = 0; j < larger.length; j++) {
    if (c[larger[j]]) {
      intersection.push(larger[j])
    }
  }

  return intersection
}

const a = []
const b = []

const maxIterations = 4000000

for (let i = 0; i < maxIterations; i++) {
  const num = i
  a.push(num)
  b.push(num)
}

//for (let i = 0; i < Math.floor(Math.random() * maxIterations); i++) {
  //b.push(Math.floor(Math.random() * maxIterations))
//}

a.sort(() => {
  return Math.random() - 0.5
})

b.sort(() => {
  return Math.random() - 0.5
})

console.time('intersection')
const result1 = 0
console.timeEnd('intersection')

console.time('intersection2')
const result2 = intersection2(a, b).length
console.timeEnd('intersection2')
console.log('Result 1:', result1)
console.log('Result 2:', result2)

// Log used memory
const used = process.memoryUsage()
const formatBytes = (bytes, decimals = 2) => {
  if (bytes === 0) return '0 Bytes'
  const k = 1024
  const dm = decimals < 0 ? 0 : decimals
  const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB']
  const i = Math.floor(Math.log(bytes) / Math.log(k))
  return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i]
}

console.log('Used memory:', formatBytes(used.heapUsed))
