<b>IMPORTANT: I don't have the rights over the <i>checker.sh</i> file and all of the test from <i>date</i> and <i>operatii</i> folder</b>, I just uploaded them in order to check the code is working properly.

# C Text Editor
<b>Homework #1</b><br>

The editor gets three files as parameters (<b>date.in</b> - containing the original text, <b>operations.in</b> - list all the operations that will be applied to the text, <b>output.out</b> - the file where the text will be written after the operations are applied over initial text).
<br>
To do this, all text is retrieved and pasted into a double-typed list, each element in the list being represented by one character in the initial text.
<br>
By using <i>preluare_operatii</i> function, the operatii.in file is analyzed and all the operations described in this file are extracted and executed. The undo and redo functionalities are provided by using two stacks; first store all operations that are different than <b>undo</b> and <b>redo</b> operations.
<br>
After all the desired changes have been made to the default text, the modified one is read one character by one and write in the <b>output.out</b> file, this being the content the user wanted to get.

# How to run the text editor
1. <code>make build</code>
2. <code>make checker</code>
<br>It should run all the tests and return a 130 points.
