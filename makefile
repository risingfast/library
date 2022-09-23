# -*- makefile -*-
# makefile for mysql-c-library project
# Created: 15-Jun-2021
# Geoffrey Jarman
# Log
#       08-Nov-2021 add bookDetails
#       09-Dec-2021 add bookDelRating
#       10-Dec-2021 add bookDelSeries
#       10-Dec-2021 add bookDetails2
#       20-Sep-2022 add .cgi targets

# $@ Target file
# $^ Dependency files
# $(CC) Compiler executable
# $(CFLAGS) Compiler options
# $@ Source file
# $^ Dependent files (dependencies)

# Variables defined with capital letters and invoked with $(VARNAME) in any rules

CC=gcc
CFLAGS=-g -o
SQL1FLAGS=-I/usr/include/mysql
SQL2FLAGS=-L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl -lresolv

all: mysql-c-library bookInquiry bookInquiry.cgi bookInquiry1 bookInquiry1.cgi bookInquiry2 bookInquiry2.cgi bookDetails bookDetails.cgi bookDetails2 bookDetails2.cgi bookCharacters bookCharacters.cgi bookAddAuthor bookAddAuthor.cgi bookAddClassification bookAddClassification.cgi bookAddRating bookAddRating.cgi bookAddSeries bookAddSeries.cgi bookAddSource bookAddSource.cgi bookAddGenre bookAddGenre.cgi bookAddStatus bookAddStatus.cgi bookTitleID bookTitleID.cgi bookAddCharacter bookAddCharacter.cgi bookChgCharNme bookChgCharNme.cgi bookChgCharacter bookChgCharacter.cgi bookDelCharacter bookDelCharacter.cgi bookVldtCharId bookVldtCharId.cgi bookDelClassification bookDelClassification.cgi bookDelRating bookDelRating.cgi bookDelSeries bookDelSeries.cgi bookDelSource bookDelSource.cgi bookDelGenre bookDelGenre.cgi bookDelStatus bookDelStatus.cgi bookDelAuthor bookDelAuthor.cgi bookVldtStatusId bookVldtStatusId.cgi bookChgStatusNme bookChgStatusNme.cgi bookVldtGenreId bookVldtGenreId.cgi bookChgGenreNme bookChgGenreNme.cgi bookVldtSourceId bookVldtSourceId.cgi bookChgSourceNme bookChgSourceNme.cgi bookVldtSeriesId bookVldtSeriesId.cgi bookChgSeriesNme bookChgSeriesNme.cgi bookVldtRatingId bookVldtRatingId.cgi bookChgRatingNme bookChgRatingNme.cgi bookVldtClassId bookVldtClassId.cgi bookChgClassNme bookChgClassNme.cgi bookVldtAuthorID bookVldtAuthorID.cgi bookChgAuthorNme bookChgAuthorNme.cgi bookFetchLOVs bookFetchLOVs.cgi bookAddBook bookAddBook.cgi bookDelBook bookDelBook.cgi bookUpdtBook bookUpdtBook.cgi bookShow bookShow.cgi bookUpdate bookUpdate.cgi bookCharacterID bookCharacterID.cgi


mysql-c-library: mysql-c-library.c rf50.c cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry: bookInquiry.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry.cgi: bookInquiry.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry1: bookInquiry1.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry1.cgi: bookInquiry1.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry2: bookInquiry2.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookInquiry2.cgi: bookInquiry2.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDetails: bookDetails.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDetails.cgi: bookDetails.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDetails2: bookDetails2.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDetails2.cgi: bookDetails2.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookCharacters: bookCharacters.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookCharacters.cgi: bookCharacters.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddAuthor: bookAddAuthor.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddAuthor.cgi: bookAddAuthor.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddClassification: bookAddClassification.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddClassification.cgi: bookAddClassification.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddRating: bookAddRating.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddRating.cgi: bookAddRating.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddSeries: bookAddSeries.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddSeries.cgi: bookAddSeries.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddSource: bookAddSource.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddSource.cgi: bookAddSource.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddGenre: bookAddGenre.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddGenre.cgi: bookAddGenre.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddStatus: bookAddStatus.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddStatus.cgi: bookAddStatus.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookTitleID: bookTitleID.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookTitleID.cgi: bookTitleID.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddCharacter: bookAddCharacter.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddCharacter.cgi: bookAddCharacter.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgCharNme: bookChgCharNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgCharNme.cgi: bookChgCharNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgCharacter: bookChgCharacter.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgCharacter.cgi: bookChgCharacter.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelCharacter: bookDelCharacter.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelCharacter.cgi: bookDelCharacter.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelClassification: bookDelClassification.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelClassification.cgi: bookDelClassification.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtCharId: bookVldtCharId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtCharId.cgi: bookVldtCharId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelRating: bookDelRating.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelRating.cgi: bookDelRating.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelSeries: bookDelSeries.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelSeries.cgi: bookDelSeries.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelSource: bookDelSource.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelSource.cgi: bookDelSource.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelStatus: bookDelStatus.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelStatus.cgi: bookDelStatus.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelGenre: bookDelGenre.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelGenre.cgi: bookDelGenre.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelAuthor: bookDelAuthor.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelAuthor.cgi: bookDelAuthor.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtStatusId: bookVldtStatusId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtStatusId.cgi: bookVldtStatusId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgStatusNme: bookChgStatusNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgStatusNme.cgi: bookChgStatusNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgGenreNme: bookChgGenreNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgGenreNme.cgi: bookChgGenreNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtGenreId: bookVldtGenreId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtGenreId.cgi: bookVldtGenreId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtSourceId: bookVldtSourceId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtSourceId.cgi: bookVldtSourceId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgSourceNme: bookChgSourceNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgSourceNme.cgi: bookChgSourceNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtSeriesId: bookVldtSeriesId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtSeriesId.cgi: bookVldtSeriesId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgSeriesNme: bookChgSeriesNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgSeriesNme.cgi: bookChgSeriesNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtRatingId: bookVldtRatingId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtRatingId.cgi: bookVldtRatingId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgRatingNme: bookChgRatingNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgRatingNme.cgi: bookChgRatingNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtClassId: bookVldtClassId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtClassId.cgi: bookVldtClassId.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtAuthorID: bookVldtAuthorID.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookVldtAuthorID.cgi: bookVldtAuthorID.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgClassNme: bookChgClassNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgClassNme.cgi: bookChgClassNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgAuthorNme: bookChgAuthorNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookChgAuthorNme.cgi: bookChgAuthorNme.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookFetchLOVs: bookFetchLOVs.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookFetchLOVs.cgi: bookFetchLOVs.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddBook: bookAddBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookAddBook.cgi: bookAddBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelBook: bookDelBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookDelBook.cgi: bookDelBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookUpdtBook: bookUpdtBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookUpdtBook.cgi: bookUpdtBook.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookShow: bookShow.c 
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookShow.cgi: bookShow.c 
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookUpdate: bookUpdate.c 
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookUpdate.cgi: bookUpdate.c 
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookCharacterID: bookCharacterID.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

bookCharacterID.cgi: bookCharacterID.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

clean:
	rm -f *.o *.s *.i mysql-c-library bookInquiry bookInquiry.cgi bookInquiry1 bookInquiry1.cgi bookInquiry2 bookInquiry2.cgi bookDetails bookDetails.cgi bookDetails2 bookDetails2.cgi bookCharacters bookCharacters.cgi bookAddAuthor bookAddAuthor.cgi bookAddClassification bookAddClassification.cgi bookAddRating bookAddRating.cgi bookAddSeries bookAddSeries.cgi bookAddSource bookAddSource.cgi bookAddGenre bookAddGenre.cgi bookAddStatus bookAddStatus.cgi bookTitleID bookTitleID.cgi bookAddCharacter bookAddCharacter.cgi bookChgCharNme bookChgCharNme.cgi bookChgCharacter bookChgCharacter.cgi bookDelCharacter bookDelCharacter.cgi bookVldtCharId bookVldtCharId.cgi bookDelClassification bookDelClassification.cgi bookDelRating bookDelRating.cgi bookDelSeries bookDelSeries.cgi bookDelSource bookDelSource.cgi bookDelGenre bookDelGenre.cgi bookDelStatus bookDelStatus.cgi bookDelAuthor bookDelAuthor.cgi bookVldtStatusId bookVldtStatusId.cgi bookChgStatusNme bookChgStatusNme.cgi bookVldtGenreId bookVldtGenreId.cgi bookChgGenreNme bookChgGenreNme.cgi bookVldtSourceId bookVldtSourceId.cgi bookChgSourceNme bookChgSourceNme.cgi bookVldtSeriesId bookVldtSeriesId.cgi bookChgSeriesNme bookChgSeriesNme.cgi bookVldtRatingId bookVldtRatingId.cgi bookChgRatingNme bookChgRatingNme.cgi bookVldtClassId bookVldtClassId.cgi bookChgClassNme bookChgClassNme.cgi bookVldtAuthorID bookVldtAuthorID.cgi bookChgAuthorNme bookChgAuthorNme.cgi bookFetchLOVs bookFetchLOVs.cgi bookAddBook bookAddBook.cgi bookDelBook bookDelBook.cgi bookUpdtBook bookUpdtBook.cgi bookShow bookShow.cgi bookUpdate bookUpdate.cgi bookCharacterID bookCharacterID.cgi
