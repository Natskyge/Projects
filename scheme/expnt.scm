(define (expnt b n)
  (define (expnt-iter a b n)
    (cond ((= n 0) a)
          ((even? n)
           (expnt-iter a (* b b) (/ n 2)))
          (else
            (expnt-iter (* a b) b (- n 1)))))
  (expnt-iter 1 b n))
