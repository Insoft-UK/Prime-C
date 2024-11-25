//
//  ViewController.swift
//  HP Prime Simulator
//
//  Created by Richie on 17/11/2024.
//

import Cocoa
import Metal
import MetalKit
import CoreVideo

class ViewController: NSViewController {
    @IBOutlet weak var imageView: NSImageView!
    
    let width = 320   // New width
    let height = 240  // New height
    let bytesPerPixel = 4
    
    var metalDevice: MTLDevice!
    var commandQueue: MTLCommandQueue!
    var texture: MTLTexture!
    
    var displayLink: CVDisplayLink!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Set up Metal
        metalDevice = MTLCreateSystemDefaultDevice()
        commandQueue = metalDevice.makeCommandQueue()
        
        // Create a Metal texture
        let textureDescriptor = MTLTextureDescriptor()
        textureDescriptor.pixelFormat = .bgra8Unorm
        textureDescriptor.width = width
        textureDescriptor.height = height
        textureDescriptor.usage = [.shaderRead, .shaderWrite]
        texture = metalDevice.makeTexture(descriptor: textureDescriptor)
        
//        textureData = UnsafeMutableRawPointer.allocate(byteCount: width * height * bytesPerPixel, alignment: 1)
                
        
        // Set up CVDisplayLink
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink)
        CVDisplayLinkSetOutputCallback(displayLink, { _, _, _, _, _, context in
            let viewController = unsafeBitCast(context, to: ViewController.self)
            viewController.updateFrame()
            return kCVReturnSuccess
        }, UnsafeMutableRawPointer(Unmanaged.passUnretained(self).toOpaque()))
        CVDisplayLinkStart(displayLink)
        
        
        
        memset(displayRAM()!, 255, width * height * bytesPerPixel)
        let thread = Thread {
            CppBridge.hpprgm()
        }
        thread.start()
    }
    
    override func viewWillDisappear() {
        super.viewWillDisappear()
        CVDisplayLinkStop(displayLink)
    }
    
    func updateFrame() {
        // Update the pixel data in the texture
        updateTexture()
        
        // Render the texture and update the NSImageView
        renderToImageView()
        
        
    }
    
    func updateTexture() {
        // Create pixel data (gradient example)
//                let pixelData = UnsafeMutableRawPointer.allocate(byteCount: width * height * bytesPerPixel, alignment: 1)
//                defer { pixelData.deallocate() }
//                
//                let data = pixelData.assumingMemoryBound(to: UInt8.self)
//        for y in 0..<height {
//            for x in 0..<width {
//                let offset = (y * width + x) * bytesPerPixel
//                data[offset] = UInt8(x % 256)      // Red
//                data[offset + 1] = UInt8(y % 256)  // Green
//                data[offset + 2] = 128             // Blue
//                data[offset + 3] = 255             // Alpha
//            }
//        }
//        
        // Copy the pixel data to the Metal texture
        let region = MTLRegionMake2D(0, 0, width, height)
        if let pixelData = displayRAM() {
            texture.replace(region: region, mipmapLevel: 0, withBytes: pixelData, bytesPerRow: width * bytesPerPixel)
        }
    }
    
    func renderToImageView() {
        // Create an NSBitmapImageRep from the Metal texture
        let bitmapRep = NSBitmapImageRep(bitmapDataPlanes: nil,
                                         pixelsWide: width,
                                         pixelsHigh: height,
                                         bitsPerSample: 8,
                                         samplesPerPixel: bytesPerPixel,
                                         hasAlpha: true,
                                         isPlanar: false,
                                         colorSpaceName: .deviceRGB,
                                         bytesPerRow: width * bytesPerPixel,
                                         bitsPerPixel: bytesPerPixel * 8)

        let region = MTLRegionMake2D(0, 0, width, height)
        bitmapRep?.bitmapData?.withMemoryRebound(to: UInt8.self, capacity: width * height * bytesPerPixel) { bufferPointer in
            texture.getBytes(bufferPointer, bytesPerRow: width * bytesPerPixel, from: region, mipmapLevel: 0)
        }

        // Convert the bitmap representation to an NSImage and update the NSImageView
        if let bitmapRep = bitmapRep {
            let image = NSImage(size: NSSize(width: width, height: height))
            image.addRepresentation(bitmapRep)
            DispatchQueue.main.async {
                self.imageView.image = image
            }
        }
    }
}

