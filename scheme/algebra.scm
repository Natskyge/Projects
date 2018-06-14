(define mul-table '(('() 0 1 2 3 4 5 6)
                      (0 0 1 2 3 4 5 6)
                      (1 1 2 3 4 5 6 0)
                      (2 2 3 4 5 6 0 4)
                      (3 3 4 5 6 0 1 2)
                      (4 4 5 6 0 1 2 3)
                      (5 5 6 0 1 2 3 4)
                      (6 6 0 1 2 3 4 5)))

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
