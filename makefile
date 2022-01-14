# -*- makefile -*-
# makefile for mysql-c-library project
# Created: 15-Jun-2021
# Geoffrey Jarman
# Log
#       08-Nov-2021 add bookDetails
#       09-Dec-2021 add bookDelRating
#       10-Dec-2021 add bookDelSeries
#       10-Dec-2021 add bookDetails2
# $@ Target file
# $^ Dependency files
# $(CC) Compiler executable
# $(CFLAGS) Compiler options
# $@ Souce file
# $^ Dependent files (dependencies)

# Variables

CC=gcc
CFLAGS=-g -o
SQL1FLAGS=-I/usr/include/mysql
SQL2FLAGS=-L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl -lresolv

all: mysql-c-library bookInquiry bookInquiry1 bookInquiry2 bookDetails bookDetails2 bookCharacters bookAddAuthor bookAddClassification bookAddRating bookAddSeries bookAddSource bookAddGenre bookAddStatus bookTitleID bookAddCharacter bookChgCharNme bookDelCharacter bookVldtCharId bookDelClassification bookDelRating bookDelSeries bookDelSource bookDelGenre bookDelStatus bookDelAuthor bookVldtStatusId bookChgStatusNme bookVldtGenreId bookChgGenreNme bookVldtSourceId bookChgSourceNme bookVldtSeriesId bookChgSeriesNme bookVldtRatingId bookChgRatingNme bookVldtClassId bookChgClassNme bookVldtAuthorId bookChgAuthorNme bookFetchLOVs bookAddBook bookDelBook bookUpdtBook

mysql-c-library: mysql-c-library.c rf50.c cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry: bookInquiry.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry1: bookInquiry1.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry2: bookInquiry2.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDetails: bookDetails.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDetails2: bookDetails2.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookCharacters: bookCharacters.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddAuthor: bookAddAuthor.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddClassification: bookAddClassification.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddRating: bookAddRating.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddSeries: bookAddSeries.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddSource: bookAddSource.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddGenre: bookAddGenre.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddStatus: bookAddStatus.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookTitleID: bookTitleID.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddCharacter: bookAddCharacter.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgCharNme: bookChgCharNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelCharacter: bookDelCharacter.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelClassification: bookDelClassification.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtCharId: bookVldtCharId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelRating: bookDelRating.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelSeries: bookDelSeries.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelSource: bookDelSource.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelStatus: bookDelStatus.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelGenre: bookDelGenre.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelAuthor: bookDelAuthor.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtStatusId: bookVldtStatusId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgStatusNme: bookChgStatusNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgGenreNme: bookChgGenreNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtGenreId: bookVldtGenreId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtSourceId: bookVldtSourceId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgSourceNme: bookChgSourceNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtSeriesId: bookVldtSeriesId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgSeriesNme: bookChgSeriesNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtRatingId: bookVldtRatingId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgRatingNme: bookChgRatingNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtClassId: bookVldtClassId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtAuthorId: bookVldtAuthorId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgClassNme: bookChgClassNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgAuthorNme: bookChgAuthorNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookFetchLOVs: bookFetchLOVs.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddBook: bookAddBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelBook: bookDelBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookUpdtBook: bookUpdtBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

clean:
	rm -f *.o *.s *.i mysql-c-library bookInquiry bookInquiry1 bookInquiry2 bookDetails bookDetails2 bookCharacters bookAddAuthor bookAddClassification bookAddRating bookAddSeries bookAddSource bookAddGenre bookAddStatus bookTitleID bookAddCharacter bookChgCharNme bookDelCharacter bookVldtCharId bookDelClassification bookDelRating bookDelSeries bookDelSource bookDelGenre bookDelStatus bookDelAuthor bookVldtStatusId bookChgStatusNme bookVldtGenreId bookChgGenreNme bookVldtSourceId bookChgSourceNme bookVldtSeriesId bookChgSeriesNme bookVldtRatingId bookChgRatingNme bookVldtClassId bookChgClassNme bookVldtAuthorId bookChgAuthorNme bookFetchLOVs bookAddBook bookDelBook bookUpdtBook
