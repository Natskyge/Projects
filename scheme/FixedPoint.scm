(define (average x y)
  (/ (+ x y) 2))

(define Tolerance 0.00001)
(define (FixedPoint Function FirstGuess)
  (define (CloseEnough? v1 v2)
    (< (/ (abs (- v1 v2)) v2)
       Tolerance))
  (define (Try Guess)
    (let ((Next (Function Guess)))
      (if (CloseEnough? Guess Next)
          Next
          (begin (display Next)
                 (newline)
                 (Try Next)))))
  (Try FirstGuess))

(define (FPaverage f guess)
  (FixedPoint (lambda (x) (average x (f x))) guess))

(FixedPoint (lambda (x) (/ (log 1000) (log x))) 2.0)
(FPaverage (lambda (x) (/ (log 1000) (log x))) 2.0)
