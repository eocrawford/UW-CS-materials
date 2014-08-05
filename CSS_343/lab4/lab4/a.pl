use strict;
use warnings;
use Cwd;

# opendir(DIR, cwd()) or die "opendir; $!\n";
# my @files = grep {/.*\.h/} readdir(DIR);
# closedir(DIR);
my @files = shift;

for my $file (@files)
{
    undef my $class;
    undef my $fGatherComments;
    undef my $fPrintInclude;
    my $comment = "";
    my $cpp = $file;
    $cpp =~ s/(.*)\.h/$1\.cpp/;

    open IN, $file or die "open in; $!\n";
    open OUT, ">$cpp" or die "open out; $!\n";

    while (<IN>)
    {
        if ($fGatherComments && !$fPrintInclude)
        {
            print OUT "#include \"$file\"\n\n";
            $fPrintInclude = 1;
        }

        if (/class (.*)/)
        {
            $class = $1;
            $class =~ s/(.*) : .*/$1/;
            $fGatherComments = 1;
        }
        elsif (/(\/\/.*)/ && $fGatherComments)
        {
            $comment .= "$1\n";
        }
        elsif (/\s*(.*?)([\w\~\<\>\=\!]+\(.*);/)
        {
            my $returnType = $1;
            my $method = $2;

            $returnType =~ s/virtual //;
            $method =~ s/\((.*) = \d+\)/\($1\)/;

            if ($class && $method !~ / = 0/)
            {
                print OUT "//-----------------------------------------------------------------------------\n";
                print OUT "// $method\n";
                print OUT $comment;
                print OUT "//-----------------------------------------------------------------------------\n";
                print OUT $returnType . $class . "::" . $method . "\n";
                print OUT "{\n";

                if ($returnType =~ /bool/)
                {
                    print OUT "    return false;\n";
                }
                elsif ($returnType =~ /\*/)
                {
                    print OUT "    return NULL;\n";
                }

                print OUT "}\n\n";

                $comment = "";
            }
        }
    }

    close IN;
    close OUT;

    if (-s $cpp <= 0)
    {
        unlink $cpp;
    }
}
