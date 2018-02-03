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











; 2.18
(define (revers lst)
  (define (rev-iter lst)
	(if (null? lst)
	    '()
		(append (rev-iter (cdr lst)) (list (car lst)))))
  (rev-iter lst))

; 2.20
(define (same-parity num . lst)
  (define parity (modulo num 2))
  (define (par-iter lst)
	(cond ((null? lst) 
		   '())
		  ((= (modulo (car lst) 2) parity)
		   (cons (car lst) (par-iter (cdr lst))))
		  (#t
		   (par-iter (cdr lst)))))
  (cons num (par-iter lst)))

(define (prim-map proc items)
  (if (null? items)
	  '()
	  (cons (proc (car items))
			(prim-map proc (cdr items)))))

(define (square-lst lst)
  (prim-map (lambda (x) (* x x))
			lst))

(define (for-eac proc lst)
  (if (null? lst)
	  #t
	  (begin (proc (car lst))
			 (for-eac proc (cdr lst)))))

(define (deep-revers lst)
  (revers (prim-map revers lst)))


(define (fringe lst)
  (cond ((null? lst) '())
		((not (pair? lst)) (list lst))
		(else
		  (append (fringe (car lst))
				  (fringe (cdr lst))))))

(define (make-mobile left right)
  (list left right))

(define (make-branch len struct)
  (list len struct))

(define left-branch car)
(define right-branch cadr)
(define branch-len car)
(define branch-struct cadr)

(define test-mobile (make-mobile (make-branch 2 3) 5))

(define (total-weight mobile)
  (cond ((null? mobile) 0)
		((not (pair? mobile)) mobile)
		(else
		  (+ (total-weight (left-branch mobile))
			 (total-weight (right-branch mobile))))))

; Combines the elements of a list into one using combine and get-data
; In order to use the following must be true:
; 1: There exists a function, conc, such that:
;      conc(split1(list), split2(list)) = list
;    and
;      conc(split2(list), split1(list)) != list
;
; 2: combine(arg, get-data(item)) must be defined for item in list, where arg is
; also compatible.
;
; 3: There must be an identity element for combine
(define (combine-lst split1 split2 combine get-data lst identity)
  (cond ((null? lst) identity)
		((not (pair? lst)) (get-data lst))
		(else
		  (combine (combine-lst (split1 lst))
				   (combine-lst (split2 lst))))))

(define (square x) (* x x))

(define (square-tree-dir tree)
  (cond ((null? tree) '())
		((not (pair? tree)) (square tree))
		(else (cons (square-tree-dir (car tree))
					(square-tree-dir (cdr tree))))))

(define (square-tree tree)
  (map (lambda (sub-tree)
		 (cond ((pair? sub-tree)
				(square-tree sub-tree))
			   (#t
				(square sub-tree))))
	   tree))

(define (tree-map func tree)
  (map (lambda (sub-tree)
		 (cond ((pair? sub-tree)
				(tree-map func sub-tree))
			   (#t
				(func sub-tree))))
	   tree))

; Works because i make a set out of the removed element and those that are left.
; And since order doesn't matter it means i get all posible combies.
(define (subset s)
  (if (null? s)
	  (list '())
	  (let ((rest (subset (cdr s))))
		(append rest 
				(map (lambda (x) 
					   (append (list (car s)) x)) 
					 rest)))))

(define (accumulate op initial sequence)
  (if (null? sequence)
	  initial
	  (op (car sequence)
		  (accumulate op initial (cdr sequence)))))

(define (mapt p sequence)
  (accumulate (lambda (x y) (cons (p x) y)) '() sequence))

(define (appendt seq1 seq2)
  (accumulate cons seq2 seq1))

(define (lengtht sequence)
  (accumulate (lambda (x y) (+ 1 y)) 0 sequence))

(define (horner-eval x . coeff)
  (accumulate (lambda (this-coeff higher-terms)
				(+ this-coeff (* x higher-terms)))
			  0
			  coeff))

(define (accumulate-n op init seqs)
  (if (null? (car seqs))
	  '()
	  (cons (accumulate op init (map car seqs))
			(accumulate-n op init (map cdr seqs)))))

(define (dot-product v w)
  (accumulate + 0 (map * v w)))

;;(define (matrix-*-vector m v)
;;  (map ?? m))

;;(define (transpose mat)
;;  (accumulate-n ?? ?? mat))

;;(define (matrix-*-matrix m n)
;;  (let ((cols (transpose n)))
;;	(map ?? m)))

(define (fold-leftt op initial sequence)
  (define (iter result rest)
	(if (null? rest)
	    result
		(iter (op result (car rest))
			  (cdr rest))))
  (iter initial sequence))

;; Fold-left and fold-right will only be equal if op is commutiative
