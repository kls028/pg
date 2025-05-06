<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Full Image</title>
    <link rel="stylesheet" href="styles/gallery-style.css">
</head>
<body>
    <h2>View Image</h2>
    <div class="full-image-container">
        <img src="/images/<?php echo basename($image['filename']); ?>" alt="Full-size Image">
        <h3><?php echo $image['title']; ?></h3>
        <p>by: <?php echo $image['author']; ?></p>
    </div>
    <a href="app.php?controller=gallery&action=galleryAction">Back to gallery</a>
</body>
</html>
