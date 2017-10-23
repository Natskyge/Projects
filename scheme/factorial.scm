(define (factorial n)
  (define (fact-iter i acc)
    (if (< n i)
      acc
      (fact-iter (+ i 1) (* acc i))))
  (fact-iter 1 1))
