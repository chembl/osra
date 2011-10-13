package net.sf.osra;

import java.io.IOException;
import java.io.InputStream;
import java.io.Writer;
import java.util.PropertyResourceBundle;

import net.sf.jnati.NativeCodeException;
import net.sf.jnati.deploy.NativeLibraryLoader;

/**
 * JNI bridge for OSRA library.
 * 
 * @author <a href="mailto:dmitry.katsubo@gmail.com">Dmitry Katsubo</a>
 */
public class OsraLib {

	/**
	 * Process the given image with OSRA library. For more information see the corresponding <a href=
	 * "https://sourceforge.net/apps/mediawiki/osra/index.php?title=Usage">CLI options</a>.
	 * 
	 * @param imageData
	 *            the image binary data
	 * @param outputStructureWriter
	 *            the writer to output the found structures in given format
	 * @param format
	 *            one of the formats, accepted by OpenBabel ("sdf", "smi", "can").
	 * @param embeddedFormat
	 *            format to be embedded into SDF ("inchi", "smi", "can").
	 * @param outputConfidence
	 *            include confidence
	 * @param outputCoordinates
	 *            include box coordinates
	 * @param outputAvgBondLength
	 *            include average bond length
	 * @return 0, if the call succeeded or negative value in case of error
	 */
	public static native int processImage(byte[] imageData, Writer outputStructureWriter, String format,
			String embeddedFormat, boolean outputConfidence, boolean outputCoordinates, boolean outputAvgBondLength);

	private static final String NAME = "osra";

	private static final String VERSION;

	public static String getVersion() {
		return VERSION;
	}

	static {
		try {
			VERSION = getVersionFromResource();

			NativeLibraryLoader.loadLibrary(NAME, VERSION);
		} catch (NativeCodeException e) {
			// Unable to handle this.
			throw new RuntimeException(e);
		} catch (IOException e) {
			// Unable to handle this.
			throw new RuntimeException(e);
		}
	}

	private static final String MAVEN_PROPERTIES = "META-INF/maven/net.sf.osra/osra/pom.properties";

	/**
	 * This function looks up the OSRA version from Maven properties file.
	 */
	private static String getVersionFromResource() throws IOException {
		final InputStream is = OsraLib.class.getClassLoader().getResourceAsStream(MAVEN_PROPERTIES);

		try {
			final PropertyResourceBundle resourceBundle = new PropertyResourceBundle(is);

			return resourceBundle.getString("version");
		} finally {
			is.close();
		}
	}

	/*
	 * // Sample usage: public static void main(String[] args) throws IOException { final StringWriter writer = new
	 * StringWriter();
	 * 
	 * int result = processImage(IOUtils.toByteArray(new FileInputStream(args[0])), writer, "sdf", "inchi", true, true,
	 * true);
	 * 
	 * System.out.println("OSRA completed with result:" + result + " structure:\n" + writer.toString() + "\n"); }
	 */
}