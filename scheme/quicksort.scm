(define array '(1 7 3 4 0 2 8 5 9 6))
(define test '(-1 0 5 -10))

(define (merge lst1 lst2)
  (cond ((null? lst1) lst2)
		((null? lst2) lst1)
		((<= (car lst1) (car lst2))
		 (cons (car lst1) (merge (cdr lst1) lst2)))
		((< (car lst2) (car lst1))
		 (cons (car lst2) (merge lst1 (cdr lst2))))))

(define (split lst)
  (let loop ((tortoise lst) (hare lst) (acc '()))
    (if (or (null? hare) (null? (cdr hare)))
        (list (reverse acc) tortoise)
        (loop (cdr  tortoise) 
              (cddr hare) 
              (cons (car tortoise) acc)))))

(define (mergesort lst)
  (if (or (null? lst) (null? (cdr lst)))
	  lst
	  (merge (mergesort (car  (split lst)))
			 (mergesort (cadr (split lst))))))

(define (rand-list len)
  (if (<= len 0)
	  '()
	  (cons (random 100) (rand-list (- len 1)))))

(define (mini x y . args)
  (define (compare x y)
	(if (< x y) x y))
  (let loop ((tempmin (compare x y)) (lst args))
	(if (null? lst)
	  	tempmin
		(loop (compare tempmin (car lst)) (cdr lst)))))

(define (listlength lst)
  (if (null? lst)
	  0
	  (+ 1 (listlength (cdr lst)))))

(define (findindex i lst)
  (cond ((null? lst) '())
		((< i 1) (car lst))
		(else
		  (findindex (- i 1) (cdr lst)))))

(define (frac-approx r acc)
  (let loop ((r r)
			 (q-h (ceiling r))
			 (q-l (floor r)))
	(cond ((> acc (abs (- r q-h))) q-h)
		  ((> r (/ (+ q-h q-l) 2))
		   (loop r q-h (/ (+ q-h q-l) 2)))
		  (else
			(loop r (/ (+ q-h q-l) 2) q-l)))))
