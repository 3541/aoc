#lang racket
(define (parse path)
    (map (lambda (s) (length
                 (apply set-intersect
                        (map (lambda (s) (map string->number (string-split s #rx" +")))
                             (string-split (substring s 8) #rx" \\| ")))))
         (file->lines path)))

(writeln
 (apply + (map (curry expt 2)
               (filter
                (curryr >= 0)
                (map
                 (curryr - 1)
                 (parse "input"))))))

(struct State (index count map))

(writeln
 (State-count
  (foldl
   (lambda (count state)
     (let ([next_count (+ (State-count state) (hash-ref (State-map state) (State-index state) 1))]
           [range (in-inclusive-range (+ (State-index state) 1) (+ (State-index state) count))])
       (State
        (+ (State-index state) 1)
        next_count
        (apply (curry hash-set* (State-map state))
               (flatten
                (for/list ([i range])
                          `(,i ,(+ (hash-ref (State-map state) i 1)
                                   (hash-ref (State-map state) (State-index state) 1)))))))))
     (State 1 0 (hash))
     (parse "input"))))
