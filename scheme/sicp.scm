;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;                              Exercise 1.30 								   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
(define (summation func a b next)
  (define (summation-iter a sum)
	(if (< b a)
	  	sum
		(summation-iter (next a) (+ sum (func a)))))
  (summation-iter a 0))

(summation (lambda (x) (+ x 1))
		   0
		   10
		   (lambda (x) (+ x 1)))

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

(define (facto n)
  (let loop ((ptr 1)
			 (prod 1))
	(if (< n ptr)
	    prod
		(loop (+ ptr 1) (* prod ptr)))))
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

(define (reverss lst)
  (let loop ((res lst))
	(if (null? res)
	    '()
		(append (loop (cdr res)) (list (car res))))))

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

(define (same-parityy num . inlst)
  (let loop ((parity (modulo num 2))
			 (lst (cons num inlst)))
	(cond ((null? lst)
		   '())
		  ((= (modulo (car lst) 2) parity)
		   (cons (car lst) (loop parity (cdr lst))))
		  (else
			(loop parity (cdr lst))))))

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

; Takes  a sequenc with an initial value and accumulates using op.
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

;; Notes:
(define (flatmap proc seq)
  (accumulate append '() (map proc seq)))

(define (remove item sequence)
  (filter (lambda (x) (not (= x item)))
		  sequence))

(define (permut s)
  (if (null? s)
	  (list '())
	  (flatmap (lambda (x)
				 (map (lambda (p) (cons x p))
					  (permut (remove x s))))
			   s)))

; a <= b
(define (enum-seq a b)
  (let loop ((ptr a))
	(if (< b ptr)
	    '()
		(cons ptr (loop (+ ptr 1))))))

(define (prime? n)
  (define (F n i)
    (cond ((< n (* i i)) #t)
          ((zero? (remainder n i)) #f)
          (else
           (F n (+ i 1)))))
 (cond ((< n 2) #f)
     (else
      (F n 2))))

(define (prime-sum? pair)
  (prime? (+ (car pair) (cadr pair))))

(define (unique-pairs n)
  (flatmap (lambda (i) 
			 (map (lambda (j) (list j i))
				  (enum-seq 1 (- i 1))))
		   (enum-seq 1 n)))

(define (make-sum-triplet lst)
  (map (lambda (pair)
		 (list (car pair) (cadr pair) (+ (car pair) (cadr pair))))
	   lst))

(define (prime-sum-pairs n)
  (make-sum-triplet (filter prime-sum? (unique-pairs n))))

(define (unique-triplets n)
  (flatmap (lambda (i)
			 (map (lambda (j) (cons j i))
				  (enum-seq 1 (- (car i) 1))))
		   (unique-pairs n)))

(define (sum-to-s? triplet s)
  (= s 
	 (+ (car triplet) 
		(cadr triplet) 
		(caddr triplet))))

(define (make-sum-4let lst)
  (map (lambda (triplet)
		 (list (car triplet) 
			   (cadr triplet) 
			   (caddr triplet) 
			   (+ (car triplet) 
				  (cadr triplet) 
				  (caddr triplet))))
	   lst))

(define (sum-to-s-triplets n s)
  (make-sum-4let (filter (lambda (triplet)
						   (sum-to-s? triplet s))
				   (unique-triplets n))))

; E:2.46
(define (make-vect x y)
  (list x y))

(define xcor-vect car)
(define ycor-vect cadr)

(define (add-vect vect1 vect2)
  (make-vect (+ (xcor-vect vect1) (xcor-vect vect2))
			 (+ (ycor-vect vect1) (ycor-vect vect2))))

(define (sub-vect vect1 vect2)
  (make-vect (- (xcor-vect vect1) (xcor-vect vect2))
			 (- (ycor-vect vect1) (ycor-vect vect2))))

(define (scale-vect scale vect)
  (make-vect (* scale (xcor-vect vect))
			 (* scale (ycor-vect vect))))

(define (make-frame origin edge1 edge2)
  (list origin edge1 edge2))

(define frame-origin car)
(define frame-edge1 cadr)
(define frame-edge2 caddr)

;(define (make-frame origin edge1 edge2)
;  (list origin (cons edge1 edge2)))
;
;(define frame-origin car)
;(define (frame-edge1 frame)
;  (car (cadr frame)))
;(define (frame-edge2 frame)
;  (cadr (cadr frame)))

; p(n,k)=p(n-k,n-k)+p(n,k-1)
; p(0,k)=1
; p(1,k)=1

(define (part n)
  (define (p n k)
	(cond ((< n k) 0)
	  	  ((<= n 1) 1)
		  ((<= k 1) 1)
		  (else
			(+ (p (- n k) (- n k))
			   (p n (- k 1))))))
  (p n n))

(list 'a 'b 'c) ;=> (a b c)
(list (list 'george)) ;=> ((george))
(cdr '((x1 x2) (y1 y2))) ;=> ((y1 y2))
(car '((x1 x2) (y1 y2))) ;=> (x1 x2)
(pair? (car '(a short list))) ;=> #f
(memq 'red '((red shoes) (blue socks))) ;=> #f
(memq 'red '(red shoes blue socks)) ;=> (red shoes blue socks)

(define (equalt? a b)
  (cond ((and (symbol? a) (symbol? b)) (eq? a b))
		((and (null? a) (null? b)) #t)
		(else
		  (and (equalt? (car a) (car b)) (equalt? (cdr a) (cdr b))))))

(car ''abc) ;=> quote, because ''abc <=> (quote (quote abc))

; We assume we have the following expressions
(define (variable? x)
  (symbol? x))
(define (same-variable? x y)
  (and (variable? x) (variable? y) (eq? x y)))

(define (sum? e)
  (and (pair? e) (eq? (car e) '+)))
(define (addend e)
  (cadr e))
(define (augend e)
  (if (null? (cdddr e))
      (caddr e)
	  (append (list '+) (cddr e))))
(define (make-sum a1 a2)
  (cond ((=number? a1 0) a2)
		((=number? a2 0) a1)
		((and (number? a1) (number? a2)) (+ a1 a2))
		(else (list '+ a1 a2))))

(define (product? e)
  (and (pair? e) (eq? (car e) '*)))
(define (multiplier e)
  (cadr e))
(define (multiplicand e)
  (if (null? (cdddr e))
      (caddr e)
	  (append (list '*) (cddr e))))
(define (make-product m1 m2)
  (cond ((or (=number? m1 0) (=number? m2 0)) 0)
		((=number? m1 1) m2)
		((=number? m2 1) m1)
		((and (number? m1) (number? m2)) (* m1 m2))
		(else (list '* m1 m2))))

(define (=number? s n)
  (and (number? s) (= s n)))

(define (exponentiation? e)
  (and (pair? e) (eq? (car e) '^)))
(define (base e)
  (cadr e))
(define (exponent e)
  (caddr e))
(define (make-exponentiation e1 e2)
  (cond  ((=number? e2 0) 1)
		 ((=number? e2 1) e1)
		 ((and (number? e1) (number? e2)) (expt e1 e2))
		 (else (list '^ e1 e2))))

(define (deriv expr var)
  (cond ((number? expr) 0)
		((variable? expr)
		 (if (same-variable? expr var) 1 0))
		((sum? expr)
		 (make-sum (deriv (addend expr) var)
				   (deriv (augend expr) var)))
		((product? expr)
		 (make-sum (make-product (multiplier expr)
								 (deriv (multiplicand expr) var))
				   (make-product (deriv (multiplier expr) var)
								 (multiplicand expr))))
		((exponentiation? expr)
		 (make-product (exponent expr)
					   (make-exponentiation (base expr)
											(make-sum (exponent expr) 
													  (- 0 1)))))
		(else
		  (error "Unknown expression type -- DERIV" expr))))

(define (union-set s1 s2)
  (if (null? s1)
	  s2
	  (union-set (cdr s1) (adjoin-set (car s1) s2))))

(define (intersection-set s1 s2)
  (if (or (null? s1) (null? s2))
	  '()
	  (let ((x1 (car s1)) (x2 (car s2)))
		(cond ((= x1 x2)
			   (adjoin-set x1 (intersection-set (cdr s1) (cdr s2))))
			  ((< x1 x2)
			   (intersection-set (cdr s1) s2))
			  ((> x1 x2)
			   (intersection-set s1 (cdr s2)))))))

(define (element-of-set? x set)
  (cond ((null? set) #f)
		((< x (car set)) #f)
		((= x (car set)) #t)
		(else (element-of-set? x (cdr set)))))

(define (adjoin-set x set)
  (if (element-of-set? x set)
	  set
	  (cons x set)))
