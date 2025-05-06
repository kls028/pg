<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Upload Image - Sudoku Gallery</title>
    <link rel="stylesheet" href="styles/upload-style.css">
</head>
<body>
    <div class="upload-container">
        <h2>Upload Image</h2>
        <?php if (!empty($errors)): ?>
            <div class="error-messages">
                <?php foreach ($errors as $error): ?>
                    <p class="error"><?php echo htmlspecialchars($error); ?></p>
                <?php endforeach; ?>
            </div>
        <?php endif; ?>
        <form method="POST" enctype="multipart/form-data" action="app.php?controller=gallery&action=uploadAction" class="upload-form">
            <label for="image">Choose Image:</label>
            <input type="file" name="image" id="image" accept=".png, .jpg"required><br><br>

            <label for = "author">Author Name:</label>
            <input type="text" name="author" id="author" 
                <?php if(isset($user)) : ?>
                    placeholder="<?php echo htmlspecialchars($user); ?>"
                    value="<?php echo htmlspecialchars($user); ?>"
                <?php else : ?>
                    placeholder="Enter your name"
                <?php endif; ?>
            required><br><br>

            <label for="title">Title:</label>
            <input type="text" name="title" id="title" required><br><br>

            <label for="watermark">Watermark Text:</label>
            <input type="text" name="watermark" id="watermark" required><br><br>
            
            <label for="status">Status:</label>
            <select name="status" id="status">
                <option value="public">Public</option>
                <?php if(isset($user)): ?>
                    <option value="private">Private</option>
                <?php else:?>
                    <option value="private" disabled>Private (Login to upload private images)</option>
                <?php endif;?>
            </select><br><br>

            <button type="submit" class="upload-btn">Upload</button>
        </form>
        <div class="upload-footer">
            <p><a href="app.php?controller=gallery&action=galleryAction">Go back to the Gallery</a></p>
        </div>
    </div>
</body>
</html>
