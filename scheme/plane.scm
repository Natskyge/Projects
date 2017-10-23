(define make-segment list)

(define start-segment car)

(define end-segment cadr)

(define point list)

(define x-point car)

(define y-point cadr)

(define (average x y) (/ (+ x y) 2))
(define (midpoint-segment l)
  (point (average (x-point (start-segment l)) (x-point (end-segment l)))
         (average (y-point (start-segment l)) (y-point (end-segment l)))))

(define (print-point p)
  (display "(")
  (display (x-point p))
  (display ",")
  (display (y-point p))
  (display ")")
  (newline))

(define (make-rectangle p1 p2)
  (make-segment p1 p2))

(define (area-rectangle x)
  (* (- (x-point (end-segment x)) (x-point (start-segment x)))
     (- (y-point (end-segment x)) (y-point (start-segment x)))))

(define (perimter a)
  (+ (* 2 (- (x-point (end-segment a)) (x-point (start-segment a))))
     (* 2 (- (y-point (end-segment a)) (y-point (start-segment a))))))

(define (cons2 x y)
  (lambda (m) (m x y)))
(define (car2 z)
  (z (lambda (p q) p)))
(define (cdr2 z)
  (z (lambda (p q) q)))

; (car2 (cons2 x y))
; (car2 (lambda (m) (m x y)))
; ((lambda (m) (m x y)) (lambda (p q) p))
; ((lambda (p q) p) x y)
; x

; (cdr2 (cons2 x y))
; (cdr2 (lambda (m) (m x y)))
; ((lambda (m) (m x y)) (lambda (p q) q))
; ((lambda (p q) q) x y)
; y

(define (cons3 a b)
  (* (expt 2 a) (expt 3 b)))

(define (logA A x)
  (/ (log x) (log A)))

(define (cdr3 p)
  (define logp (logA 2 p))
  (define log23 (logA 2 3))
  (define (loop i)
    (if (= 0 (modulo (- logp (* log23 i)) 1))
        i
        (loop (+ i 1))))
  (loop 0))

(define zero (lambda (f) (lambda (x) x)))
(define (add-1 n)
  (lambda (f) (lambda (x) (f ((n f) x)))))

; (add-1 zero)
; (add-1 (lambda (f) (lambda (x) x)))
; (lambda (f) (lambda (x) (f (((lambda (f) (lambda (x) x)) f) x))))
; (lambda (f) (lambda (x) (f ((x) x))))

