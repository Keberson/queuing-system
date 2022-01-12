## Theory

Let's consider the keywords included in the formulation of the problem.
We are talking about discrete systems – this means that the system can be described by a set of discrete values. The discrete nature of the description of the state of the system as a whole allows, firstly, to reduce its model to a set of interaction rules that are logical and algorithmic in nature and do not require solving ordinary differential, algebraic-
differential equations or partial differential equations characteristic of other types of mathematical and computer modeling. Secondly, the change in the state of a discrete system is conditionally instantaneous, i.e. in the process of modeling, it is possible to identify only individual moments of time at which
events occur that change the state of the system. And in the process of modeling, it is necessary and sufficient to consider the behavior of the system only at these points in time.

Therefore, the modeling procedure is based on a discrete-event mechanism for promoting model time. It consists of iteratively changing procedures, which are traditionally called phases.
1. In the `timer correction phase`, a transition is made to the next moment in time when at least one event is supposed to be implemented.
2. In the `viewing phase`, when the model time is stopped, system state changes are performed and, if necessary, events  related to future changes are predicted.
3. In the `input phase`, the initialization of the system state is performed and the initial prediction of events is performed.

From the point of view of the data model, it turns out to be convenient to use an auxiliary data structure describing the current status of the passage of the application (which in terms of simulation modeling
is usually called a transaction) of the processing route involving system resources. Interacting with resources, the transaction changes their state, and hence the state of the system as a whole. For example, in GPSS, such
a data structure contains the following fields:

- _transaction ID_;
- _priority of the transaction_;
- _the time of occurrence of the next event related to the transaction_;
- _the current block where the transaction is located_;
- _the next block where the transaction should be moved_.

The considered data structures are combined into sets (lists), which can be such typical containers as linked lists, queues, priority queues and trees. Traditionally, in
simulation modeling, these lists are called chains. At least they talk about two chains – the chain of future events (FEC) and the chain of current events (CEC).

In the timer correction phase, a record with a minimum forecast time is located in the chain of future events, the model time is switched to this moment and all records with the specified time are transferred from FEC to CEC. In the viewing phase, the transaction progresses along the processing route (for example, GPSS - the line numbers through which the transaction passes change). This continues until one of the following situations occurs:
- the transaction fully completed the service – in this case, the corresponding entry is simply removed from the circuit of current events, the allocated memory is freed (this is important because when modeling systems with a large number of queries may require the allocation of significant amounts of memory; by the way, for the
same reason, the new transaction from the same source are introduced into the system one at a time, not scheduled time points input all
transaction for the whole planned period of the simulation);
- the transaction initiated an operation that requires time – in this case, the predicted time for
performing this operation is added to the current model time, this value is fixed in
the corresponding field of the data structure discussed above, and it is transferred from CEC to FEC;
- the transaction claims to occupy some resource that is currently occupied/not available (or is waiting for some logical condition that is not currently being fulfilled) – in the simplest case, the transaction remains in CEC, but a sign of the need to promote the transaction "as early as possible" is prescribed as a moment in time; then, with each change in the state of the system due to the promotion of other transactions, the conditions for the promotion of transactions marked with the sign "as early as possible" will be analyzed; in a more complex case, transactions are moved from CEC to separate chains that can be associated with a specific resource; this is more rational, since it is not necessary to analyze all pending transactions in the system, and when the state of the resource changes, only the chain associated with it is queried; in addition, the introduction of separate chains can be initiated by the developer to implement more complex service disciplines (different from the classic FIFO queue).
