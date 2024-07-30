(define (domain minecraft)
  (:requirements :strips :typing)
  (:types block)
  
  (:predicates
    (at ?b - block ?x ?y)      
    (holding ?b - block)
    (on ?b1 - block ?b2 - block)
  )
  
  (:action pick-up ; we can pick-up blocks in minecraft
    :parameters (?b - block ?x ?y)
    :precondition (and (at ?b ?x ?y) (not (holding ?b)))
    :effect (and (not (at ?b ?x ?y)) (holding ?b))
  )
  
  (:action put-down ; we can put-down blocks in minecraft as well :)
    :parameters (?b - block ?x ?y)
    :precondition (and (holding ?b) (not (at ?b ?x ?y)))
    :effect (and (at ?b ?x ?y) (not (holding ?b)))
  )
  
  (:action stack ; we can put one block on another block
    :parameters (?b1 ?b2 - block ?x ?y)
    :precondition (and (holding ?b1) (at ?b2 ?x ?y))
    :effect (and (on ?b1 ?b2) (not (holding ?b1)))
  )
  
  (:action unstack ; we can take one block from another as well :)
    :parameters (?b1 ?b2 - block ?x ?y)
    :precondition (and (on ?b1 ?b2) (at ?b2 ?x ?y))
    :effect (and (not (on ?b1 ?b2)) (holding ?b1))
  )
)
