# Post-Offices-and-Semaphores

Simulation of behavior of customers in a post office.
Each  of  your  customers  will  be  simulated  by  a separate  thread  created  by  main  program.
Customers arriving at the post office will wait until a clerk is available then leave the post office when they are done.
Your post office should have a single FIFO queue that  you  will  represent  using  a  mutex  and  a condition variable.