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

(define (merge-sort lst)
  (define part (split lst))
  (if (or (null? lst) (null? (cdr lst)))
	  lst
	  (merge (merge-sort (car part))
			 (merge-sort (cadr part)))))

(define (rand-list len)
  (if (<= len 0)
	  '()
	  (cons (random 100) (rand-list (- len 1)))))

(define (revers lst)
  (let loop ((lst1 lst) (acc '()))
	(if (null? lst1)
	    acc
		(loop (cdr lst1) (cons (car lst1) acc)))))

(define (mini x y . args)
  (if (null? args)
	  (if (< x y)
		  x
		  y)
	  (car (merge-sort (cons x (cons y args))))))

(define (median x y . args)
  (if (null? args)
	  (/ (+ x y) 2)
	  ))

(define (factorial n)
  (if (= n 0)
	  1
	  (* n (factorial (- n 1)))))

; (let loop ((var1 start1)
;            (var2 start2)
;            ...
;            (varN startN))
;   (function body))

; Is the same as:

; (define (loop var1 var2 ... varN)
;   (function body))
; (loop start1 start2 ... startN)
