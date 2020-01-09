# Centralized Multi-User Concurrent Bank Account Manager:

I have implemented a Centralized Multi-User Concurrent Bank Account Manager. The system has two important components.

### Bank Server: 
The server program that services online requests for account
manipulations and maintains all customer records correctly.

### Clients: 
Customers are clients of the bank server and use its services to update bank
accounts. The operations that can be performed on an account are: withdrawal of an
amount from an account and deposit of an amount into an account. Additionally, the
bank server can have it's own service that periodically deposits an interest amount to each
account based on some fixed rate.

## Server:

The server receives queries from customers for operations on accounts. The server has following
functionalities:
Able to accept multiple concurrent customer requests (i.e., is multi-
threaded)
Provides locking/protection for access to an account records during shared
access (i.e., a user might be depositing money into his account and at the same time an
online billing agent might be withdrawing money from the same account). Such cases are handled by protecting variables in the critical section.
Maintain correctness of records at each record, (i.e., allow withdrawal from an account
only if it has sufficient funds etc.)
To create a set of records at the server initially, I have used an input file(accounts.txt) that the server
reads and creates account information. You can generate your own input file.

## Client:

A client issues requests to the server from a transaction based on account numbers. Client functionality:
Issue withdrawal or deposit requests.
