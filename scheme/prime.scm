(define (prime-factor n)
  (define (fact-iter k facts q)
	(cond ((prime? k)
		   (append facts (list k)))
		  ((= 0 (modulo k q))
		   (fact-iter (/ k q) 
					  (append facts (list q))
					  2))
		  (else
			(fact-iter k facts (+ q 1)))))
  (fact-iter n '() 2))

(define (prime? n)
  (let loop ((d 2))
    (cond ((< n (* d d)) #t)
          ((zero? (modulo n d)) #f)
          (else (loop (+ d 1))))))
