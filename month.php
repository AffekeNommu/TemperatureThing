<?php
echo '<html>
<head></head>
<body>
<a href="index.php">Day</a>
<form action="monthdisp.php">
<p>
Which Month?
<select name="month">
  <option value="1" ';
  if (date("F")=="January"){echo "selected";};
  echo '>January</option>
  <option value="2" ';
  if (date("F")=="February"){echo "selected";};
  echo '>February</option>
  <option value="3" ';
  if (date("F")=="March"){echo "selected";};
  echo '>March</option>
  <option value="4" ';
  if (date("F")=="April"){echo "selected";};
  echo '>April</option>
  <option value="5" ';
  if (date("F")=="May"){echo "selected";};
  echo '>May</option>
  <option value="6" ';
  if (date("F")=="June"){echo "selected";};
  echo '>June</option>
  <option value="7" ';
  if (date("F")=="July"){echo "selected";};
  echo '>July</option>
  <option value="8" ';
  if (date("F")=="August"){echo "selected";};
  echo '>August</option>
  <option value="9" ';
  if (date("F")=="September"){echo "selected";};
  echo '>September</option>
  <option value="10" ';
  if (date("F")=="October"){echo "selected";};
  echo '>October</option>
  <option value="11" ';
  if (date("F")=="November"){echo "selected";};
  echo '>November</option>
  <option value="12" ';
  if (date("F")=="December"){echo "selected";};
  echo '>December</option>
</select>
</p>
<p>
Which Year?
<select name="year">
  <option value="2017" ';
  if (date("Y")=="2017"){echo "selected";};
  echo '>2017</option>
  <option value="2018" ';
  if (date("Y")=="2018"){echo "selected";};
  echo '>2018</option>
  <option value="2019" ';
  if (date("Y")=="2019"){echo "selected";};
  echo '>2019</option>
  <option value="2020" ';
  if (date("Y")=="2020"){echo "selected";};
  echo '>2020</option>
</select>
</p>
<input type="submit">
</form>
</body>
</html>';
?>
