backend:  backend.o soBay.o
	gcc backend.o soBay.o SOBay_common/users_lib.o -o backend/Backend 

backend.o: backend/backend.c
	gcc -lpthread -c backend/backend.c

frontend:  frontend.o soBay.o
	gcc frontend.o soBay.o -o frontend/Frontend

frontend.o: frontend/frontend.c
	gcc -lpthread -c frontend/frontend.c


promotor:  promotor.o soBay.o
	gcc promotor.o soBay.o -o promotor/Promotor

promotor.o: promotor/promotor.c
	gcc -c promotor/promotor.c


soBay.o: SOBay_common/soBay.c SOBay_common/soBay.h SOBay_common/users_lib.h
	gcc -c SOBay_common/soBay.c 


clean:
	rm *.o backend/Backend frontend/Frontend

cleanall:
	rm *.o backend/Backend frontend/Frontend promotor/Promotor

all: backend frontend