(define (problem minecraft-tower)
  (:domain minecraft)
  (:objects block1 block2 block3 - block
            x1 x2 y1 y2 - block)
  
  (:init
    (at block1 x1 y1) (at block2 x2 y1) (at block3 x1 y2)
  )
  
  (:goal (and (on block3 block2) (on block2 block1)))
)
