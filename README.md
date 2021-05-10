# Algorithms and Principles of Computer Science
### Project of the year 2019 in C language   
## PROJECT  
The aim of the project is to design and implement in C language, using standard libraries, a sort of social network where there are entities capable of creating relationships between them.<br />
Each entity is characterized by an id and contains all its relationships, that are unidirectional, to the other entities.<br />
All relationships are characterized by an id, which identifies their type, and are linked to the recipient of the relationship.<br />

## STRUCTURE  
The project was developed using multiple double linked lists in order to make efficient the insertion / deletion of entities, the creation / removal of relationships and the printing of the entities-relationships report at a given moment.<br />

## COMMANDS  
The list of commands is the following:<br />
| Command | Description |
| --- | --- |
| addent | adds an entity identified by "ent_id" to the set of monitored entities; if the entity is already monitored, it does nothing |
| delent | eliminates the entity identified by "ent_id" from the set of monitored entities; it deletes all relations of which "ent_id " is a part (both as origin and as destination) |
| addrel | adds a relationship (identified by "rel_id") between the entities "ent_s" and "ent_d", where "ent_d" is the recipient of the report. If the relationship between "ent_d" and "ent_s" already exists, or if at least one of the entities is not monitored, it does nothing. The monitoring of the relationship type "rel_id" implicitly begins with the first "addrel" command that affects it |
| delrel | deletes the relationship identified by "rel_id" between the entities "ent_s" and "ent_d" (where "ent_d" is the receiver of the relationship); if there is no "rel_id" relationship between "ent_s" and "ent_d" (with "ent_d" as receiver), it does nothing |
| report | outputs the list of relations, reporting for each entity with the largest number of incoming relations |
| end | ends the command sequence |<br />

**Syntax**
- addent ent_id
- delent ent_id
- addrel ent_s ent_d rel_id
- delrel ent_s ent_d rel_id
- report
- end

## COMPILATION, EXECUTION AND TESTING  
You can test the program by compiling and then running the [source](https://github.com/DanieleGiorgianni/Algorithms_and_Principles_of_Computer_Science-Project_2019/blob/main/project/source.c) file, entering command line inputs, or using a text file.<br />
To compile the source, use the following statement:<br />
`gcc -Wmaybe-uninitialized -Wuninitialized -Wall -pedantic -Werror -g3 source.c -o source`<br />
To execute the program and testing it (maybe using these public [tests](https://github.com/DanieleGiorgianni/Algorithms_and_Principles_of_Computer_Science-Project_2019/tree/main/test) as input and their related outputs), execute the two commands respectively:<br />
`cat input.txt | ./source > output.txt`<br />
`diff output.txt public_output.txt`<br />
<br />For more details see the [specifications](https://github.com/DanieleGiorgianni/Algorithms_and_Principles_of_Computer_Science-Project_2019/blob/main/specs/Prova%20Finale%20API.pdf) (in Italian).<br />
