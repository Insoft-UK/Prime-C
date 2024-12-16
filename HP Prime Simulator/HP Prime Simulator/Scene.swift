/*
 The MIT License (MIT)
 
 Copyright (c) 2024 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

import SpriteKit

class Scene: SKScene {
    private var mutableTexture: SKMutableTexture?
    private var sprite: SKSpriteNode?
    
    let width = 320
    let height = 240
    let bytesPerPixel = 4
    
    override func didMove(to view: SKView) {
        mutableTexture = SKMutableTexture(size: CGSize(width: width, height: height))
        sprite = SKSpriteNode(color: .red, size: CGSize(width: width, height: height))
        sprite?.texture = mutableTexture;
        sprite?.anchorPoint = .zero;
        sprite?.yScale = -1;
        sprite?.position.y = CGFloat(height);
        addChild(sprite!)
    }
    
    // MARK: - Keyboard Events
    override func keyDown(with event: NSEvent) {
        
    }
    
    override func touchesBegan(with event: NSEvent) {
        
    }
    
    // MARK: - Mouse Events
    override func mouseDown(with event: NSEvent) {
        
    }
    
    override func moveUp(_ sender: Any?) {
        
    }
    
    override func mouseMoved(with event: NSEvent) {
        
    }
    
    // MARK: - Update

    override func update(_ currentTime: TimeInterval) {
        mutableTexture?.modifyPixelData({ pixelData, lengthInBytes in
            pixelData?.copyMemory(from: displayRAM(), byteCount: lengthInBytes)
        })
    }
}
