;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;                              Exercise 1.30 								   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
(define (summation func a b next)
  (define (summation-iter a sum)
	(if (< b a)
	  	sum
		(summation-iter (next a) (+ sum (func a)))))
  (summation-iter a 0))

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;                              Exercise 1.29								   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
(define (simpsons func strt stop acc)
  (define h (/ (- stop strt)
			   acc))
  (define (y k)
	(func (+ strt (* k h))))
  (define (yk k)
	(cond ((or (<= k 0) (>= k acc))
		   	(y k))
		  ((even? k)
		   	(* 2 (y k)))
		  (else
		   	(* 4 (y k)))))
  (* (summation yk 0 acc (lambda (x) (+ x 1)))
	 (/ h 3.0)))

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;          Exercise 1.31, totally not just a copy-paste of 1.30				   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
(define (product func a b next)
  (define (product-iter a sum)
	(if (< b a)
	  	sum
		(product-iter (next a) (* sum (func a)))))
  (product-iter a 1))

(define (factorial n)
  (product (lambda (x) x)
		   1
		   n
		   (lambda (x) (+ x 1))))

; doesn't work, fix later
(define (pi-approx acc)
  (define (square x) (* x x))
  (* (/ (square (product (lambda (x) x)
						 4.0
						 (* 2 acc)
						 (lambda (x) (+ x 2.0))))
		(square (product (lambda (x) x)
						 3.0
						 (+ (* 2 acc) 1)
						 (lambda (x) (+ x 2.0)))))
	 8))

; Recursive product - not tested but probaly works
(define (prod-rec func a b next)
  (if (> a b)
	  1
	  (* (func a)
		 (prod-rec func (next a) b next))))

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;                              Exercise 1.32								   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
(define (accumulate combiner null-value term a next b)
  (define (accumulate-iter i sum)
	(if (< b i)
	    sum
		(accumulate-iter (next i) (combiner sum (term i)))))
  (accumulate-iter a null-value))

; Sum:
(define (sum-accumulate func a b next)
  (accumulate (lambda (x y) (+ x y)) 0 func a next b))

; Product:
(define (product-accumulate func a b next)
  (accumulate (lambda (x y) (* x y)) 0 func a next b))

; Recursive version
(define (accumulate-recursive combiner null-value term a b next)
  (if (> a b)
	  null-value
	  (combiner (term a)
				(accumulate-recursive combiner null-value 
									  term (next a) b next))))

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;								Exercise 1.33								   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
(define (filtered-accumulate combiner term filt3r next null-value a b)
  (define (filtered-accumulate-iter sum i)
	(cond ((< b i) sum)
		  ((filt3r i)
		   	(filtered-accumulate-iter (combiner sum (term i)) (next i)))
		  (else
		   	(filtered-accumulate-iter sum (next i)))))
  (filtered-accumulate-iter null-value a))

;a)
;Broken
(define (prime? n)
  (define sqrt-n (sqrt n))
  (define test 0)
  (define (loop n k i j)
	(set! test (+ (* 6 k) 1))
	(cond ((<= sqrt-n test) #t)
		  ((= (modulo n test)) #f)
		  (#t (loop n (+ k (modulo i j)) (* i -1) (+ j i)))))
  (cond ((<= n 1) #f)
		((and (< 1 n) (<= n 3)) #t)
		((or (= (modulo n 3) 0) (= (modulo n 2))) #f)
		(#t (loop n 1 1 1))))

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;									                     Notes			              					   	 ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
; This is the same
(define (f x y)
  ((lambda (a b)
	 (+ (* x a a)
		(* y b)
		(* a b)))
   (+ 1 (* x y))
   (- 1 y)))
; As this
(define (f x y)
  (let ((a (+ 1 (* x y)))
		(b (- 1 y)))
	(+ (* x (square a))
	   (* y b)
	   (* a b))))
; so what happens is the let copies in the definitions of a and b into the
; expression. Like with lambda, but in reverse.

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;								Exercise 1.34								   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
(define (f g) (g 2))
; (f f)
; (f 2)
; (2 2)
; error because 2 is not a function
