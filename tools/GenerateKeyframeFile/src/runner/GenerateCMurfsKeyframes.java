package runner;

import creator.CMurfKeyframeFile;

public class GenerateCMurfsKeyframes {
	public static void main(String[] args) throws Exception {
		if ( (args.length < 2) || (args[0].equals("-h"))){
			System.out.println("Usage: java GenerateCMurfsKeyframes <input filename> <output filename>");
			System.out.println();			
			System.exit(0);
		}
		
		String inputFile = args[0];
		String outputFile = args[1];
		
		CMurfKeyframeFile cmurfOutput = new CMurfKeyframeFile(inputFile, outputFile);
	}

	
}
