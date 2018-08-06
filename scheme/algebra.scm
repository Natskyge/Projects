(define mul-table '(('() 00 01 02 10 11 12)
					 (00 00 01 02 10 11 12)
					 (01 01 02 00 11 12 10)
					 (02 02 00 01 12 10 11)
					 (10 10 11 12 00 01 02)
					 (11 11 12 10 01 02 00)
					 (12 12 10 11 02 00 01)))

(define (mul a b)
  (define (find-row a)
	(let loop ((lst (cdr mul-table)))
	  (if (equal? (caar lst) a)
		  (car lst)
		  (loop (cdr lst)))))
  (let loop ((row (cdr (find-row a)))
			 (top (cdr (car mul-table))))
	(if (equal? (car top) b)
		(car row)
		(loop (cdr row) (cdr top)))))


; Complex numbers.
(define (attach-tag type-tag contents)
  (cons type-tag contents))

(define (type-tag datum)
  (if (pair? datum)
	  (car datum)
	  (error "Bad tagged datum -- TYPE-TAG" datum)))

(define (contents datum)
  (if (pair? datum)
	  (cdr datum)
	  (error "Bad tagged datum -- CONTENTS" datum)))


(define (rectangular? z)
  (eq? (type-tag z) 'rectangular))

(define (polar? z)
  (eq? (type-tag z) 'polar))


(define real-part-rectangular
  (lambda (z) (car z)))

(define imag-part-rectangular
  (lambda (z) (cdr z)))

(define magnitude-rectangular
  (lambda (z)
	(sqrt (+ (expt (real-part-rectangular z) 2)
			 (expt (imag-part-rectangular z) 2)))))

(define angle-rectangular
  (lambda (z)
	(atan (real-part-rectangular z)
		  (imag-part-rectangular z))))

(define (make-from-real-imag-rectangular x y)
  (attach-tag 'rectangular (cons x y)))

(define (make-from-mag-ang-rectangular r a)
  (attach-tag 'rectangular
			  (cons (* r (cos a)) (* r (sin a)))))


(define real-part-polar)
(define imag-part-polar)
(define magnitude-polar)
(define angle-polar)
(define make-from-real-imag-polar)
(define make-from-mag-ang-polar)


(define real-part)
(define imag-part)
(define magnitude)
(define angle)
(define make-from-real-imag)
(define make-from-mag-ang)
; (make-from-real-imag (real-part z) (imag-part z)) => z
; (make-from-mag-ang (magnitude z) (angle z)) => z

(define (add-complex z1 z2)
  (make-from-real-imag (+ (real-part z1) (real-part z2))
					   (+ (imag-part z1) (real-part z2))))

(define (sub-complex z1 z2)
  (make-from-real-imag (- (real-part z1) (real-part z2))
					   (- (real-part z1) (real-part z2))))

(define (mul-complex z1 z2)
  (make-from-mag-ang (* (magnitude z1) (magnitude z2))
					 (+ (angle z1) (angle z2))))

(define (div-complex z1 z2)
  (make-from-mag-ang (/ (magnitude z1) (magnitude z2))
					 (- (angle z1) (angle z2))))

; Data directed programming
; Idea: Make operation and type table lookups to achieve generic producers.
(put proc-name proc-type proc) ; Adds a column to the operation/type table.
(get operation type) ; Finds the operation appropriate to the type.
(define (apply-generic op . args)
  (let ((type-tags (map type-tag args)))
	(let ((proc (get op type-tags)))
	  (if proc
		  (apply proc (map contents args))
		  (error
			"No method for these types -- APPLY-GENERIC"
			(list op type-tags))))))

(define (add x y) (apply-generic 'add x y))
(define (sub x y) (apply-generic 'add x y))
(define (mul x y) (apply-generic 'add x y))
(define (div x y) (apply-generic 'add x y))

(define (install-scheme-number-package)
  (define (tag x)
	(attach-tag 'scheme-number x))
  (put 'add '(scheme-number scheme-number)
	   (lambda (x y) (tag (+ x y))))
  (put 'sub '(scheme-number scheme-number)
	   (lambda (x y) (tag (- x y))))
  (put 'mul '(scheme-number scheme-number)
	   (lambda (x y) (tag (* x y))))
  (put 'div '(scheme-number scheme-number)
	   (lambda (x y) (tag (/ x y))))
  (put 'make 'scheme-number
	   (lambda (x) (tag x)))
  'done)

(define (make-scheme-number n)
  ((get 'make  'scheme-number) n))

(define (install-rational-package)
  ;; Internal procedures
  (define (numer x) (car x))
  (define (denom x) (cdr x))
  (define (make-rat n d)
	(let ((g (gcd n d)))
	  (cons (/ n g) (/ d g))))
  (define (add-rat x y)
	(make-rat (+ (* (numer x) (denom y))
				 (* (numer y) (denom x)))
			  (* (denom x) (denom y))))
  (define (sub-rat x y)
	(add-rat x (make-rat (- 0 (numer y)) (- 0 (denom y)))))
  (define (mul-rat x y)
	(make-rat (* (numer x) (numer y))
			  (* (denom x) (denom y))))
  (define (div-rat x y)
	(mul-rat x (make-rat (denom y) (number y))))

  ; Interface
  (define (tag x) (attach-tag 'rational x))
  (put 'add '(rational rational)
	   (lambda (x y) (tag (add-rat x y))))
  (put 'sub '(rational rational)
	   (lambda (x y) (tag (sub-rat x y))))
  (put 'mul '(rational rational)
	   (lambda (x y) (tag (mul-rat x y))))
  (put 'div '(rational rational)
	   (lambda (x y) (tag (div-rat x y))))

  (put 'make 'rational
	   (lambda (n d) (tag (make-rat n d))))
  'done)

(define (make-rational n d)
  ((get 'make 'rational) n d))



(define (add x y . args)

  (define (add-nums-helper lst)
	(if (null? lst)
		0
		(+ (car lst) (add-nums-helper (cdr lst)))))

  (define (add-nums lst)
	(let ((n (add-nums-helper lst)))
	  (if (= n 0)
		  '()
		  (list n))))

  (define (count-sym sym lst)
	(length (filter (lambda (x) (eq? sym x)) lst)))

  (define (add-sym sym lst)
	(let ((len (count-sym sym lst)))
	  (cond ((= len 1) sym)
			(else (list '* len sym)))))

  (define (add-syms lst)
	(if (null? lst)
		'()
		(cons (add-sym (car lst) lst) 
			  (add-syms (filter (lambda (x) (not (eq? (car lst) x))) lst)))))

  (let ((argv (append (list x y) args)))
	(let ((syms (filter symbol? argv))
		  (nums (filter number? argv)))
	  (let ((result (append (list '+) (add-syms syms) (add-nums nums))))
		(if (null? (cddr result))
			(cadr result)
			result)))))


(define (install-sparse-polynomial-package)
  ;; Internal procedures
  ;; Representation of poly
  (define (make-poly variable term-list)
	(cons variable term-list))
  (define (variable p) (car p))
  (define (term-list p) (cdr p))
  (define (variable? x) (symbol? x))
  (define (same-variable? v1 v2)
	(and (variable v1) (variable v2) (eq? v1 v2)))

  ;; Representation of terms and term lists
  (define (adjoin-term term term-list)
	(if (=zero? (coeff term))
	    term-list
		(cons term term-list)))

  (define (the-empty-termlist) '())
  (define (first-term term-list) (car term-list))
  (define (rest-terms term-list) (cdr term-list))
  (define (empty-termlist? term-list) (null? term-list))

  (define (make-term order coeff) (list order coefff))
  (define (order term) (car term))
  (define (coeff term) (cdr term))

  (define (add-terms l1 l2)
	(cond ((empty-termlist? l1) l2)
		  ((empty-termlist? l2) l1)
		  (else
			(let ((t1 (first-term l1)) (t2 (first-term l2)))
			  (cond ((> (order t1) (order t2))
					 (adjoin-term
					   t1 (add-terms (rest-terms l1) l2)))
					((< (order t1) (order t2))
					 (adjoin-term
					   t2 (add-terms l1 (rest-terms l2))))
					(else
					  (adjoin-term
						(make-term (order t1)
								   (add (coeff t1) (coeff t2)))
						(add-terms (rest-terms l1)
								   (rest-terms l2)))))))))
  (define (mul-terms l1 l2)
	(if (emptry-termlist? l1)
	    (the-empty-termlist)
		(add-terms (mul-terms-by-all-terms (first-term l1) l2)
				   (mul-terms (rest-terms l1) l2))))
  (define (mul-by-all-terms t1 l)
	(if (empty-termlist? l)
	    (the-empty-termlist)
		(let ((t2 (first-term l)))
		  (adjoin-term
			(make-term (+ (order t1) (order t2))
					   (mul (coeff t1) (coeff t2)))
			(mul-terms-by-all-terms t1 (rest-terms l))))))
  (define (add-poly p1 p2)
	(if (same-variable? (variable p1) (variable p2))
		(make-poly (variable p1)
				   (add-terms (term-list p1)
							  (term-list p2)))
		(error "Polys not in same var -- ADD-POLY"
			   (list p1 p2))))
  (define (mul-poly p1 p2)
	(if (same-variable? (variable p1) (variable p2))
		(make-poly (variable p1)
				   (mul-terms (term-list p1)
							  (term-list p2)))
		(error "Polys not in the same var -- MUL-POLY"
			   (list p1 p2))))

  (define (sparse-poly->dense-poly poly)
	(let loop ((term-list (term-list poly)) (acc (the-empty-termlist)) (n 0))
	  (cond ((empty-termlist? term-list) ((get 'make 'dense-poly) (variable poly) acc))
			((= (order (first-term term-list)) n)
			 (loop (rest-terms term-list) 
				   (append acc (coeff (first-term term-list)))
				   (+ 1 n)))
			(else
			  (loop term-list (append acc '(0)) (+ 1 n))))))

  ;; Interface to the res of the system
  (define (tag p) (attach-tag 'sparse-poly p))
  (put 'add '(sparse-poly sparse-poly)
	   (lambda (p1 p2) (tag (add-poly p1 p2))))
  (put 'mull '(sparse-poly sparse-poly)
	   (lambda (p1 p2) (tag (mul-poly p1 p2))))
  (put 'make 'sparse-poly
	   (lambda (var terms) (tag (make-poly var terms))))
  (put '=zero? 'sparse-poly
	   (lambda (p) (and (=zero? (coeff (first-term p)))
						(= 0 (order (first-term p))))))
  (put-coercion 'sparse-poly 'dense-poly sparse-poly->dense-poly)
  'done)

(define (install-dense-polynomial-package)
  ;; Internal procedures
  ;; Representation of poly
  (define (make-poly variable term-list)
	(cons variable term-list))
  (define (variable p) (car p))
  (define (term-list p) (cdr p))
  (define (variable? x) (symbol? x))
  (define (same-variable? v1 v2)
	(and (variable v1) (variable v2) (eq? v1 v2)))

  ;; Representation of terms and term lists
  (define (append-term term-list term)
	(append term-list (list term)))

  (define (the-empty-termlist) '())
  (define (first-term term-list) (car term-list))
  (define (rest-terms term-list) (cdr term-list))
  (define (empty-termlist? term-list) (null? term-list))
  (define (termlist-length term-list) (length term-list))
  (define (make-term term) term)

  (define (add-terms l1 l2)
	(cond ((empty-termlist? l1) l2)
		  ((empty-termlist? l2) l1)
		  ((< (termlist-length l1) (termlist-length l2))
		   (add-terms (append-term l1 0) l2))
		  ((> (termlist-length l1) (termlist-length l2))
		   (add-terms l1 (append-term l2 0)))
		  ((= (termlist-length l1) (termlist-length l2))
		   (map add l1 l2))))

  (define (mul-terms l1 l2)
	(if (emptry-termlist? l1)
	    (the-empty-termlist)
		(add-terms (mul-terms-by-all-terms (first-term l1) l2)
				   (mul-terms (rest-terms l1) l2))))
  (define (mul-terms-by-all-terms t l)
	(cond ((=zero? t) (the-empty-termlist))
		  ((empty-termlist? l) (the-empty-termlist))
		  (else
			(map (lambda (x) (mul t x)) l))))

  (define (add-poly p1 p2)
	(if (same-variable? (variable p1) (variable p2))
		(make-poly (variable p1)
				   (add-terms (term-list p1)
							  (term-list p2)))
		(error "Polys not in same var -- ADD-POLY"
			   (list p1 p2))))
  (define (mul-poly p1 p2)
	(if (same-variable? (variable p1) (variable p2))
		(make-poly (variable p1)
				   (mul-terms (term-list p1)
							  (term-list p2)))
		(error "Polys not in the same var -- MUL-POLY"
			   (list p1 p2))))

  (define (dense-poly->sparse-poly poly)
	(let loop ((terms (term-list poly)) (acc (the-empty-termlist)) (n 0))
	  (cond ((empty-termlist? terms) ((get 'make 'sparse-poly) (variable poly) acc))
			((=zero? (first-term terms) )
			 (loop (rest-terms terms) acc (+ n 1)))
			(else
			  (loop (rest-terms terms)
					(cons (list n (first-term terms)) acc)
					(+ n 1))))))

  ;; Interface to the res of the system
  (define (tag p) (attach-tag 'dense-poly p))
  (put 'add '(dense-poly dense-poly)
	   (lambda (p1 p2) (tag (add-poly p1 p2))))
  (put 'mull '(dense-poly dense-poly)
	   (lambda (p1 p2) (tag (mul-poly p1 p2))))
  (put 'make 'dense-poly
	   (lambda (var terms) (tag (make-poly var terms))))
  (put '=zero? 'dense-poly
	   (lambda (p) (and (=zero? (coeff (first-term p)))
						(= 0 (order (first-term p))))))
  (put-coercion 'dense-poly 'sparse-poly dense-poly->sparse-poly)
  'done)
