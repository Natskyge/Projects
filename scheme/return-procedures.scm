(define (AverageDamp f)
  (lambda (x) (Average x (f x))))

(define (Average x y)
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

(define (Deriv f)
  (define dx 0.00001)
  (lambda (x) (/ (- (f (+ x dx)) (f x))
                 dx)))

(define (NewtonTransform g)
  (lambda (x) (- x (/ (g x) ((Deriv g) x)))))
(define (NewtonsMethod g guess)
  (FixedPoint (NewtonTransform g) guess))

(define (sqrt x)
  (NewtonsMethod
   (lambda (y) (- (* y y) x)) 1.0))

(define (Cubic a b c)
  (lambda (x) (+ (* x x x)
                 (* a x x)
                 (* b x)
                 c)))

(define (CubicRoots a b c)
  (NewtonsMethod (Cubic a b c) 1.0))

(define (Double procedure)
  (lambda (x) (procedure (procedure x))))

(define (Compose f g)
  (lambda (x) (f (g x))))

(define (NthAppplication f n)
  (define (ApplicationItter nthf i)
    (cond ((<= i 0)
           nthf)
          (#t
           (ApplicationItter (Compose f nthf) (- i 1)))))
  (ApplicationItter (lambda (x) x) n))

(define (Smooth f)
  (define (Average x y z)
    (/ (+ x y z) 3))
  (define dx 0.00001)
  (lambda (x) (Average (f (+ x dx)) (f x) (f (- x dx)))))

(define (n-Smooth f n)
  ((NthAppplication Smooth n) f))


(define (NthRoot x n)
  (define logB
    (lambda (x B)
      (/ (log x) (log B))))
  (define N (floor (logB n 2)))
  (FixedPoint ((NthAppplication AverageDamp N)
                (lambda (y) (/ x (expt y (- n 1)))))
              1.0))

(define (IterativeImprove GoodEnough? Improve)
  (define (IterativeImprove-iter x)
    (if (GoodEnough? x)
        x
        (IterativeImprove-iter (Improve x))))
  (lambda (Guess) (IterativeImprove-iter Guess)))
