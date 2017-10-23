(define (loop lst func)
  (cond ((not (pair? lst)) '())
        (#t
          (cons (func (car lst))
                (loop (cdr lst) func)))))

(define (c-loop init con after action)
  (define (lst-gen init con after)
    (if (con init)
        (cons init
              (lst-gen (after init)
                       con
                       after))
        '()))
  (loop (lst-gen init con after)
        action))

(define (square-loop start stop)
  (c-loop start
          (lambda (x) (<= x stop))
          (lambda (n) (+ n 1))
          (lambda (k) (* k k))))

(define (cube-loop start stop)
  (c-loop start
	  	  (lambda (x) (<= x stop))
	  	  (lambda (n) (+ n 1))
	  	  (lambda (k) (* k k k k))))

(define (atom? x)
  (not (or (pair? x) (null? x))))

(define (in a A)
  (cond ((or (not (pair? A))) #f)
		((= a (car A)) #t)
		(#t (in a (cdr A)))))

(define (subset A B)
  (cond ((atom? A) #f)
		((not (pair? A)) #t)
		((eqv? A B) #t)
		((in (car A) B) (subset (cdr A) B))
		(#t #f)))

(define (last l)
  (cond ((null? (cdr l)) (car l))
        (#t (last (cdr l)))))

(define graph '(1 2 3 4))
(define (compose A B)
  (cond ((not (pair? A)) B)
        ((not (pair? B)) A)
        ((not (and (subset A graph) (subset B graph))) '())
        ((= (last A) (car B)) (list (car A) (last B)))
        (#t A)))

(compose '(1 2) (compose '(2 3) '(3 4)))
(compose (compose '(1 2) '(2 3)) '(3 4))
