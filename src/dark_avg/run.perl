#!/usr/bin/perl

$data_file = $ARGV[1];
@program_names = ("dark_avg");
%program_nodes = ("dark_avg", 3);

$program_to_run = $ARGV[0];
if (!$program_to_run || !$program_nodes{$program_to_run}) {
  die "Must enter program name to run. Possible programs are: " .
      "\n@program_names\n";
} else {
  if ($ENV{"MPIRUN"}) {
    $mpirun = $ENV{"MPIRUN"};
  } else {
    $mpirun = "mpirun";
  }
  if ($ENV{"MPI_HOSTS"}) {
    $hosts = "-f " . $ENV{"MPI_HOSTS"};
  } else {
    $hosts = "";
  }

  print "$mpirun -n $program_nodes{$program_to_run} $hosts $program_to_run $data_file\n";
  system("$mpirun -n $program_nodes{$program_to_run} $hosts $program_to_run $data_file");
}
