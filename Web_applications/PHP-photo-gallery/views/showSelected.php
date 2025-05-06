<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Selected Images</title>
    <link rel="stylesheet" href="styles/gallery-style.css">
</head>
<body>
    <h2>Selected Images</h2>
    <form method="POST" action="app.php?controller=gallery&action=deleteSelectedAction">
        <div class="gallery-container">
            <?php if (!empty($selectedImages)): ?>
                <?php foreach ($selectedImages as $image): ?>
                    <div class="image-card">
                        <a href="app.php?controller=gallery&action=viewImageAction&filename=<?php echo urlencode($image['filename']); ?>">
                            <img src="/images/<?php echo basename($image['filename']); ?>" alt="Image" width="200">
                        </a>
                        <h3><?php echo htmlspecialchars($image['title']); ?></h3>
                        <p>by: <?php echo htmlspecialchars($image['author']); ?></p>
                        <input type="checkbox" name="selected_images[]" value="<?php echo htmlspecialchars($image['_id']); ?>"> 
                    </div>
                <?php endforeach; ?>
            <?php else: ?>
                <p>No images selected.</p>
            <?php endif; ?>
        </div>
        <?php if (!empty($selectedImages)): ?>
            <button type="submit" class="delete-btn">Delete Selected</button>
        <?php endif; ?>
    </form>
    <a href="app.php?controller=gallery&action=galleryAction" class="navbar-link">Back to Gallery</a>
</body>
</html>
