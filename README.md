The code provided utilizes a top-down parsing method called Recursive Descent Parsing. 
Recursive descent parsing is a parsing technique where each non-terminal in the grammar has a corresponding parsing function. 
The parsing functions are recursively called to match the grammar rules and construct the parse tree.

In the given code, the Parser class uses recursive descent parsing to analyze the syntax of the input code. 
Each parsing function in the Parser class corresponds to a non-terminal in the grammar. 
For example, the parseStatement function handles statements, the parseExpression function handles expressions, and so on.

Recursive descent parsing follows a predictive parsing approach, where each parsing function predicts the next grammar production to apply based on the current lookahead token. 

By using recursive descent parsing, the code provides a straightforward and readable way to analyze the syntax of the input code, as each parsing function corresponds to a specific grammar rule.

Group Project Details
This project is a collaborative effort by a group of developers. Here are the details of the team members and their contributions:
Heyeman Abdissa     ID: ETS0632/13
Hikma Anwar         ID: ETS0633/13
Hunde Desalegn      ID: ETS0643/13
Ifa Tolla           ID: ETS0647/13
Iman Ahmed          ID: ETS0648/13
Senait Mengesha     ID: ETS1602/13
