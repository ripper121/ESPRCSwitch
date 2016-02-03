package layout;

import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.RemoteViews;

import com.example.ripper121.automationwidget.R;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Implementation of App Widget functionality.
 */
public class AutomationWidget extends AppWidgetProvider {

    private static final String switch0On = "com.example.ripper121.automationwidget.action.switch0On";
    private static final String switch0Off = "com.example.ripper121.automationwidget.action.switch0Off";
    private static final String switch1On = "com.example.ripper121.automationwidget.action.switch1On";
    private static final String switch1Off = "com.example.ripper121.automationwidget.action.switch1Off";
    private static final String switch2On = "com.example.ripper121.automationwidget.action.switch2On";
    private static final String switch2Off = "com.example.ripper121.automationwidget.action.switch2Off";
    private static final String switch3On = "com.example.ripper121.automationwidget.action.switch3On";
    private static final String switch3Off = "com.example.ripper121.automationwidget.action.switch3Off";
    private static final String switch4On = "com.example.ripper121.automationwidget.action.switch4On";
    private static final String switch4Off = "com.example.ripper121.automationwidget.action.switch4Off";
    private static final String switch5On = "com.example.ripper121.automationwidget.action.switch5On";
    private static final String switch5Off = "com.example.ripper121.automationwidget.action.switch5Off";
    private static final String switch6On = "com.example.ripper121.automationwidget.action.switch6On";
    private static final String switch6Off = "com.example.ripper121.automationwidget.action.switch6Off";
    private static final String switch7On = "com.example.ripper121.automationwidget.action.switch7On";
    private static final String switch7Off = "com.example.ripper121.automationwidget.action.switch7Off";
    private static final String switch8On = "com.example.ripper121.automationwidget.action.switch8On";
    private static final String switch8Off = "com.example.ripper121.automationwidget.action.switch8Off";
    private static final String switch9On = "com.example.ripper121.automationwidget.action.switch9On";
    private static final String switch9Off = "com.example.ripper121.automationwidget.action.switch9Off";
    private String outText = "";
    private String gatewayURL = "";
    /*
    private static String getMessage() {
        return String.valueOf(mCount++);
    }*/

    private PendingIntent getPendingSelfIntent(Context context, String action) {
        // An explicit intent directed at the current class (the "self").
        Intent intent = new Intent(context, getClass());
        intent.setAction(action);
        return PendingIntent.getBroadcast(context, 0, intent, 0);
    }

    @Override
    public void onUpdate(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds) {
        super.onUpdate(context, appWidgetManager, appWidgetIds);
        //String message = getMessage();

        // Loop for every App Widget instance that belongs to this provider.
        // Noting, that is, a user might have multiple instances of the same
        // widget on
        // their home screen.
        for (int appWidgetID : appWidgetIds) {
            RemoteViews remoteViews = new RemoteViews(context.getPackageName(), R.layout.automation_widget);

            remoteViews.setTextViewText(R.id.textView_output, outText);
            remoteViews.setOnClickPendingIntent(R.id.switch0On, getPendingSelfIntent(context, switch0On));
            remoteViews.setOnClickPendingIntent(R.id.switch0Off, getPendingSelfIntent(context, switch0Off));
            remoteViews.setOnClickPendingIntent(R.id.switch1On, getPendingSelfIntent(context, switch1On));
            remoteViews.setOnClickPendingIntent(R.id.switch1Off, getPendingSelfIntent(context, switch1Off));
            remoteViews.setOnClickPendingIntent(R.id.switch2On, getPendingSelfIntent(context, switch2On));
            remoteViews.setOnClickPendingIntent(R.id.switch2Off, getPendingSelfIntent(context, switch2Off));
            remoteViews.setOnClickPendingIntent(R.id.switch3On, getPendingSelfIntent(context, switch3On));
            remoteViews.setOnClickPendingIntent(R.id.switch3Off, getPendingSelfIntent(context, switch3Off));
            remoteViews.setOnClickPendingIntent(R.id.switch4On, getPendingSelfIntent(context, switch4On));
            remoteViews.setOnClickPendingIntent(R.id.switch4Off, getPendingSelfIntent(context, switch4Off));
            remoteViews.setOnClickPendingIntent(R.id.switch5On, getPendingSelfIntent(context, switch5On));
            remoteViews.setOnClickPendingIntent(R.id.switch5Off, getPendingSelfIntent(context, switch5Off));
            remoteViews.setOnClickPendingIntent(R.id.switch6On, getPendingSelfIntent(context, switch6On));
            remoteViews.setOnClickPendingIntent(R.id.switch6Off, getPendingSelfIntent(context, switch6Off));
            remoteViews.setOnClickPendingIntent(R.id.switch7On, getPendingSelfIntent(context, switch7On));
            remoteViews.setOnClickPendingIntent(R.id.switch7Off, getPendingSelfIntent(context, switch7Off));
            remoteViews.setOnClickPendingIntent(R.id.switch8On, getPendingSelfIntent(context, switch8On));
            remoteViews.setOnClickPendingIntent(R.id.switch8Off, getPendingSelfIntent(context, switch8Off));
            remoteViews.setOnClickPendingIntent(R.id.switch9On, getPendingSelfIntent(context, switch9On));
            remoteViews.setOnClickPendingIntent(R.id.switch9Off, getPendingSelfIntent(context, switch9Off));
            appWidgetManager.updateAppWidget(appWidgetID, remoteViews);

        }
    }

    /**
     * A general technique for calling the onUpdate method,
     * requiring only the context parameter.
     *
     * @author John Bentley, based on Android-er code.
     * @see <a href="http://android-er.blogspot.com
     * .au/2010/10/update-widget-in-onreceive-method.html">
     * Android-er > 2010-10-19 > Update Widget in onReceive() method</a>
     */
    private void onUpdate(Context context) {
        AppWidgetManager appWidgetManager = AppWidgetManager.getInstance(context);

        // Uses getClass().getName() rather than MyWidget.class.getName() for
        // portability into any App Widget Provider Class
        ComponentName thisAppWidgetComponentName = new ComponentName(context.getPackageName(), getClass().getName());
        int[] appWidgetIds = appWidgetManager.getAppWidgetIds(thisAppWidgetComponentName);
        onUpdate(context, appWidgetManager, appWidgetIds);
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        super.onReceive(context, intent);

        SharedPreferences settings = context.getSharedPreferences("PREFS_NAME", 0);
        gatewayURL = settings.getString("gatewayURL", "");
        if (switch0On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch0=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch0Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch0=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch1On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch1=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch1Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch1=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch2On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch2=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch2Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch2=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch3On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch3=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch3Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch3=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch4On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch4=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch4Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch4=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch5On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch5=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch5Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch5=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch6On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch6=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch6Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch6=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch7On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch7=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch7Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch7=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch8On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch8=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch8Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch8=off");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch9On.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch9=on");
            outText = gatewayURL;
            onUpdate(context);
        }

        if (switch9Off.equals(intent.getAction())) {
            new DownloadWebpageTask().execute(gatewayURL + "/?switch9=off");
            outText = gatewayURL;
            onUpdate(context);
        }
    }

    private String downloadUrl(String myurl) throws IOException {
        InputStream is = null;
        // Only display the first 500 characters of the retrieved
        // web page content.
        int len = 500;

        try {
            URL url = new URL(myurl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setReadTimeout(10000 /* milliseconds */);
            conn.setConnectTimeout(15000 /* milliseconds */);
            conn.setRequestMethod("GET");
            conn.setDoInput(true);
            // Starts the query
            conn.connect();
            int response = conn.getResponseCode();
            is = conn.getInputStream();
            // Convert the InputStream into a string
            //String contentAsString = readIt(is);

            Log.d("DEBUG_TAG", "GET Successfully");
            return "true";

            // Makes sure that the InputStream is closed after the app is
            // finished using it.
        } finally {
            if (is != null) {
                is.close();
            }
        }
    }

    public String readIt(InputStream stream) throws IOException, UnsupportedEncodingException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
        StringBuilder result = new StringBuilder();
        String line;
        while ((line = reader.readLine()) != null) {
            result.append(line);
        }
        return result.toString();
    }

    private class DownloadWebpageTask extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... urls) {

            // params comes from the execute() call: params[0] is the url.
            try {
                Log.d("DEBUG_TAG", "urls[0]: " + urls[0]);
                String content = null;
                if (urls[0] != null)
                    content = downloadUrl(urls[0]);
                else
                    content = "Empty";
                Log.d("DEBUG_TAG", "URL:" + content);
                return content;
            } catch (IOException e) {
                Log.d("DEBUG_TAG", "URL: " + "Unable to retrieve web page. URL may be invalid.");
                return "Unable to retrieve web page. URL may be invalid.";
            }
        }

        // onPostExecute displays the results of the AsyncTask.
        @Override
        protected void onPostExecute(String result) {
            //textView.setText(result);
        }
    }


}

