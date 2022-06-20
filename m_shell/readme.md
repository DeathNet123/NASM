# Handling the control command

<hr>
<p>We will scanning the input first from this regex to handle if there is any logical operator such as && or ||
This can be done by using the following regex:
</p>
<br>
<code>(.+)(&&|'|''|')(.+)</code>
<br>
