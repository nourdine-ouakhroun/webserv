#!/usr/bin/perl

# Print HTML header
print "<html>";
print "<head>";
print "<title>Test Perl CGI</title>";
print "</head>";
print "<body>";

my $arg = $ARGV[$i];
print "<h3>Hello My name is: $ARGV[$i++]</h3>";
print "<h3>Age: $ARGV[$i++]</h3>";
print "<h3>Project: $ARGV[$i++]</h3>";
print "<h3>City: $ARGV[$i++]</h3>";
print "<h3>State: $ARGV[$i++]</h3>";
print "<h3>Country: $ARGV[$i++]</h3>";
print "<h3>Habbits: $ARGV[$i++]</h3>";
print "<h3>Joke: $ARGV[$i++]</h3>";
print "<h3>Feedback: $ARGV[$i++]</h3>";

print "</body>";
print "</html>";
